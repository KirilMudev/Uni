#include "stdafx.h"
#include "iostream"
#include "fstream"

using namespace std;

int calculateOffSetZeroes(int const& width);
int calculateSize(int const& width, int const& height, int const& offSetZeroes);

int main()
{
	char* filename = "9x7bmp.bmp";
	char* output = "output.bmp";

	ifstream ifs(filename, ios::in | ios::binary);
	ofstream ofs(output, ios::out | ios::binary);
	

	int width = 5; //DESTINATION WIDTH
	int height = 3; //DESTINATION HEIGHT
	int offsetZeroes = calculateOffSetZeroes(width);
	int sizeOnDisk = calculateSize(width, height, offsetZeroes) + 54; //byte number 3 in header
	int numberOfBytes = calculateSize(width, height, offsetZeroes); //byte 34 in header

	//test
	cout << width << " " << height << " " << offsetZeroes << " " << sizeOnDisk << endl;

	int x = 3; //user coordinate X
	int y = 5; //user coordinate Y
	char buff[54];
	ifs.read(buff,54);
	ofs.write(buff,54); //copy old header 
	//change consequently the bytes in the new header
	ofs.seekp(2,ios::beg);
	ofs.write((char*)&sizeOnDisk,sizeof(sizeOnDisk));
	ofs.seekp(18,ios::beg);
	ofs.write((char*)&width,sizeof(width));
	ofs.seekp(22,ios::beg);
	ofs.write((char*)&height,sizeof(height));
	ofs.seekp(34,ios::beg);
	ofs.write((char*)&numberOfBytes,sizeof(numberOfBytes));

	//seek to end of header in both files
	int HEADER = 54;
	ofs.seekp(HEADER,ios::beg);
	ifs.seekg(HEADER,ios::beg);

	
	//source img data: (hardcoded here, but will be taken from source.bmp's header)
	int sourceW = 9; //byte 18
	int sourceH = 7; //byte 22
	int sourceOffSetZeroes = calculateOffSetZeroes(sourceW); //calculated from byte 18

	//NOTE: we use 3 as default number of bytes.
	//EASY 
	int counter = 0;
	while(counter != (sourceH - (y + height-1)))
	{
		ifs.seekg(sourceW*3+sourceOffSetZeroes, ios::cur);
		counter++;
	}
	counter = 0;
	while(counter != height)
	{
		char bigBuffer[8192];
		ifs.seekg((x-1)*3,ios::cur);
		ifs.read(bigBuffer,width*3);

		ofs.write(bigBuffer,width*3);
		for (int i = 0; i < offsetZeroes; i++)
		{
			char zero = 0;
			ofs.write((char*)&zero, sizeof(char));
		}
		ifs.seekg((sourceW - (x+width-1))*3 + sourceOffSetZeroes, ios::cur);
		counter++;
	}

	ofs.close();
	ifs.close();

	return 0;
}

int calculateOffSetZeroes(int const& width)
{
	int offsetZeroes = 0;
	while(!((width*3 + offsetZeroes) % 4 == 0))
	{
		offsetZeroes++;
	}

	return offsetZeroes;
}

int calculateSize(int const& width, int const& height, int const& offSetZeroes)
{
	return offSetZeroes*height + (width*height*3);
}