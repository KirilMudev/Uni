#include "stdafx.h"
#include "iostream"
#include "fstream"

using namespace std;

bool readFileName(istream &ifs, char* buffer);
bool readInt(istream &ifs, int &value);
void readError(char* filename, int const& line);

int const BUFFER_SIZE = 256;

int main()
{
	char* filename = "mytdffile.tdf";
	ifstream ifs(filename);

	int lineCounter = 0;

	if(ifs)
	{
		while(!ifs.eof())
		{
			char c;
			while (ifs.get(c) && c != ' ')
			{
				lineCounter++;
				if(c == ';')
				{
					while(ifs.get(c) && c != '\n');
				}
				else
				{
					ifs.seekg(-1, ios::cur);
					char sourceImageName[BUFFER_SIZE];
					char destImageName[BUFFER_SIZE];
					int x;
					int y;
					int width;
					int height;

					if(!(readFileName(ifs, sourceImageName) && //try to read source image
							readInt(ifs, x) && readInt(ifs, y) && readInt(ifs, width) && readInt(ifs, height))) //try to read ints
					{
						readError(filename, lineCounter);
						continue;
					}
					
					while(ifs.get(c) && c != ')') //skip to destination image
					{
						if(c == '\n' || ifs.eof()) 
						{
							readError(filename, lineCounter);
							continue;
						}
					}

					if (!readFileName(ifs, destImageName)) 
					{
						readError(filename, lineCounter);
						continue;
					}

					ifs.ignore(); //pointer is on the beginning of the next row 

					//IF HERE, THEN EVERYTHING WAS READ PROPERLY.
					//cout test
					cout << sourceImageName << " " << x << " " << y << " " << width << " " << height << " " << destImageName << endl;

					//NOW THE BITMAP PART
					try
					{
						ifstream source(sourceImageName, ios::in | ios::binary); //edit source.exception
						ofstream destination(destImageName, ios::out | ios::binary); //same here

						//read W and H from header of bmp????

						//move pointer to (x,y)

					}
					catch(...)
					{
						cout << "Something went wrong/generic error, still not finished." << endl;
					}


				}
			}

		}
	}
	else
	{
		cout << "Error opening: " << filename << endl;
	}

	ifs.close();
	
	return 0;
}


bool readFileName(istream &ifs, char* buffer)
{
	bool canContinueFlag = true;
	char c;

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
			while(ifs.get(c) && c != '\n');
			canContinueFlag = false;
			break; 
		}
	}

	return canContinueFlag;
}

bool readInt(istream &ifs, int &value)
{
	bool successfullyRead = false;

	ifs >> value;
	while(ifs.fail())
	{
		ifs.clear();
		ifs.ignore();

		if(ifs.eof() || ifs.peek() == '\n')
		{
			ifs.ignore();
			break;
		}
		
		if(ifs >> value) 
		{
			successfullyRead = true;
			break;
		}
	}

	return successfullyRead;
}

void readError(char* filename, int const& line)
{
	cout << "Error parsing data in: " << filename << " on line: " << line << endl;
}