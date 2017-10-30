#include <iostream>
#include <fstream>

const int FILENAME_BUFFER_SIZE = 256;
const int HDR_POS_FULLSIZE = 2;
const int HDR_POS_HDRSIZE = 10;
const int HDR_POS_WIDTH = 18;
const int HDR_POS_HEIGHT = 22;
const short int HDR_POS_BITSPERPIXEL = 28;
const int HDR_POS_SIZEINBYTES = 34;

int calculateRowSize(int sourceWidth, short int bitsPerPixel);
int calculateOffSetZeroes(int width, double pixelSizeInBytes);
int calculateRawBitmap(int rowSize, int heightInPixels);
bool cropBMP(char* source, int x, int y, int destWidth, int destHeight, char* destination);
bool readFileName(std::ifstream &ifs, char* buffer);
bool readParameters(std::ifstream& ifs, int &x, int &y, int &width, int &height);
bool readInt(std::ifstream &ifs, int &value);
void readError(char* filename, int line);
void readTDF(char* filename);
bool isBMP(std::ifstream& ifs);
int calcRow(int width, int bitsPerPixel);

int main(int __argc, char* __argv[])
{
	if(__argc < 2)
	{
		std::cout << "Not enough arguments. You need to supply at least one .tdf file.";
		exit(1);
	}
	else
	{
		for (int i = 1; i < __argc; i++)
		{
			readTDF(__argv[i]);
		} 
	} 

	system("pause");
	return 0;
}

void readTDF(char* filename)
{
	std::ifstream ifs(filename);

	if(ifs)
	{
		int lineCounter = 0;

		while(!ifs.eof())
		{
			lineCounter++;
			char c;
			ifs >> c;

			if(c == ';' || c == '\n') 
			{
				while(ifs.get(c) && c != '\n');
			}
			else
			{
				ifs.seekg(-1, std::ios::cur);

				char sourceImageName[FILENAME_BUFFER_SIZE];
				char destImageName[FILENAME_BUFFER_SIZE];
				int x;
				int y;
				int width;
				int height;

				if(!(readFileName(ifs, sourceImageName)))
				{
					readError(filename, lineCounter);
					while(ifs.get(c) && c != '\n');
					continue;
				}

				if(!(readParameters(ifs, x, y, width, height)))
				{
					readError(filename, lineCounter);
					while(ifs.get(c) && c != '\n');
					continue;
				}

				if(!(readFileName(ifs, destImageName)))
				{
					readError(filename, lineCounter);
					while(ifs.get(c) && c != '\n');
					continue;
				}

				//all read, skip to next line
				while(ifs.get(c) && c != '\n');

				//test cout
				//std::cout << sourceImageName << " " << x << " " << y << " " << width << " " << height << " " << destImageName << '\n';

				//NOW THE BITMAP PART
				if (!cropBMP(sourceImageName, x, y, width, height, destImageName))
				{
					std::cout << "File: " << filename << " Line: " << lineCounter << '\n'; 
				}			
			}		
		}

		ifs.close();
	}
	else
	{
		std::cout << "Error opening: " << filename << std::endl;
	}
}

bool cropBMP(char* source, int x, int y, int destWidth, int destHeight, char* destination)
{
	std::ifstream ifs(source, std::ios::in | std::ios::binary);
	
	if(ifs)
	{
		if(!isBMP(ifs))
		{
			std::cout << "File: " << source << " is not a valid bitmap image." << '\n';
			return false;
		}
		else
		{
			int sourceWidth, sourceHeight;
			ifs.seekg(HDR_POS_WIDTH, std::ios::beg);
			ifs.read((char*)&sourceWidth, sizeof(sourceWidth));
			ifs.seekg(HDR_POS_HEIGHT, std::ios::beg);
			ifs.read((char*)&sourceHeight, sizeof(sourceHeight));

			if(x < 0 || x > sourceWidth || y < 0 || y > sourceHeight)
			{
				std::cout << "Invalid starting coordinates.";
				return false;
			}
			else if(((x + destWidth) > sourceWidth) || ((y + destHeight) > sourceHeight))
			{
				std::cout << "Invalid thumbnail size. ";
				return false;
			}

			//read source info from header
			int headerSize;
			ifs.seekg(HDR_POS_HDRSIZE, std::ios::beg);
			ifs.read((char*)&headerSize, sizeof(headerSize));
			
			short int bitsPerPixel;
			ifs.seekg(HDR_POS_BITSPERPIXEL, std::ios::beg);
			ifs.read((char*)&bitsPerPixel, sizeof(bitsPerPixel)); 

			ifs.seekg(0, std::ios::beg); //return to beginning

			double pixelSizeInBytes = (double)bitsPerPixel / 8; //magic tricks

			int sourceRowSize = calcRow(sourceWidth, bitsPerPixel);	

			//time for ofstream
			std::ofstream ofs(destination, std::ios::out | std::ios::binary);
			
			if(ofs)
			{
				int destOffSetZeroes = calculateOffSetZeroes(destWidth, pixelSizeInBytes);
				int destRowSize = calcRow(destWidth, bitsPerPixel);

				int destRawBitmap = destHeight * destRowSize;
				int destFullSize = destRawBitmap + headerSize;

				//copy header
				char * header = new char[headerSize];
				ifs.read(header, headerSize);
				ofs.write(header, headerSize);
			
				delete[] header;

				//adjust new header
				ofs.seekp(HDR_POS_FULLSIZE, std::ios::beg);
				ofs.write((char*)&destFullSize, sizeof(destFullSize));
				ofs.seekp(HDR_POS_WIDTH, std::ios::beg);
				ofs.write((char*)&destWidth, sizeof(destWidth));
				ofs.seekp(HDR_POS_HEIGHT, std::ios::beg);
				ofs.write((char*)&destHeight, sizeof(destHeight));
				ofs.seekp(HDR_POS_SIZEINBYTES, std::ios::beg);
				ofs.write((char*)&destRawBitmap, sizeof(destRawBitmap));
			
				//seek to end of header
				ofs.seekp(headerSize, std::ios::beg);

				//cut procedure	
				char * rowBuffer = new char[destRowSize](); //will be full of 0s. should put in try block
				int counter = 0;
				while (counter != destHeight)
				{
					ifs.seekg(headerSize + sourceRowSize*(y + counter), std::ios::beg);
					ifs.seekg((int)(x*pixelSizeInBytes), std::ios::cur);
					ifs.read(rowBuffer, (destRowSize-destOffSetZeroes));
					ofs.write(rowBuffer, destRowSize);
					counter++;			
				}

				delete[] rowBuffer;
				ofs.close();
				ifs.close();
			}
			else
			{
				std::cout << "Error creating file: " << destination<< '\n';
				return false;
			}
			
		}	
	}
	else
	{
		std::cout << "Error opening: " << source << '\n';
		return false;
	}
	
	return true;
}

int calculateOffSetZeroes(int width, double pixelSizeInBytes)
{
	int offset = 0;

	while ((int)(width*pixelSizeInBytes + offset) % 4 != 0)
	{
		offset++;
	}

	return offset;
}

bool readFileName(std::ifstream& ifs, char* buffer)
{
	ifs >> buffer;
	for (int i = 0; buffer[i]; i++)
	{
		if(buffer[i-1] == '\\' && buffer[i] == ';')
		{
			int len = strlen(buffer);
			for (int k = i-1; k < len-1; k++)
			{
				buffer[k] = buffer[k+1];	
			}
			len--;
			buffer[len] = '\0';
		}
		else if(buffer[i-1] != '\\' && buffer[i] == ';')
		{
			return false;
		}
	}

	return true;
}

bool readInt(std::ifstream& ifs, int& value)
{
	while(!(ifs >> value))
	{
		ifs.clear();

		if(ifs.peek() == '\n' || ifs.eof() || ifs.peek() == ';')
		{
			return false;
		}
		
		ifs.ignore();
	}

	return true;
}

bool readParameters(std::ifstream& ifs, int& x, int& y, int& width, int& height)
{ 
	//parentheses must be present - otherwise the line is not valid
	char c;
	ifs >> c;
	if(c != '(') 
	{
		return false;
	}

	if(!(readInt(ifs, x) && readInt(ifs, y) && readInt(ifs, width) && readInt(ifs, height)))
	{
		return false;
	}

	ifs >> c;
	if(c != ')') 
	{
		return false;
	}
						
	return true;				
}

void readError(char* filename, int line)
{
	std::cout << "Error parsing data in: " << filename << " on line: " << line << std::endl;
}

bool isBMP(std::ifstream& ifs)
{
	char buffer[3];
	ifs.read(buffer, 2);
	buffer[2] = '\0';
	if(!strcmp(buffer, "BM"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int calcRow(int width, int bitsPerPixel)
{
	//formula from documentation
	return ((width*bitsPerPixel + 31) / 32) * 4;
}