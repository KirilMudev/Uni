#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "DynamicArray.h"
#include "BinaryTree.h"

using namespace std;

const int COMMAND_BUFFER_SIZE = 32;
const int DATA_BUFFER_SIZE = 128;

void readFile(DynamicArray<Entry> &arr, char* filename);
void interpret(BinaryTree<Entry> &tree);

int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		DynamicArray<Entry> entries;
		readFile(entries, argv[1]);
		BinaryTree<Entry> tree(entries); 
		interpret(tree);
	}
	else
	{
		cout << "No file provided!" << endl;
	}
	
	return 0;
}

void readFile(DynamicArray<Entry> &arr, char* filename)
{
	ifstream is(filename, ios::binary);
	
	if(is)
	{
		while(!is.eof())
		{
			Entry entr;
			int size;
			int key;

			is.read((char*)&key, sizeof(key));
			entr.setKey(key);

			is.read((char*)&size, sizeof(size));
			char* buffer = new char[size+1];

			is.read(buffer, size);
			buffer[size] = '\0';
			entr.setData(buffer);

			arr.add(entr);
			delete[] buffer;
			buffer = nullptr;
		}
	}
	else
	{
		cout << "Error opening file: " << filename << endl;
	}

	is.close();
}

void interpret(BinaryTree<Entry> &tree)
{
	char commandBuffer[COMMAND_BUFFER_SIZE];
	char data[DATA_BUFFER_SIZE];
	int key;

	while(!cin.eof()) //works for ctrl+z (?)
	{
		cin >> commandBuffer;

		if(!strcmp(commandBuffer, "add"))
		{
			cin >> key;
			cin.ignore(); //don't put more whitespace pls 
			cin.getline(data, DATA_BUFFER_SIZE-1);
			tree.add(Entry(key, data));
			//cout << "Success adding!" << endl;
		} 
		else if(!strcmp(commandBuffer, "search"))
		{
			cin >> key;
			cin.ignore();
			cin.getline(data, DATA_BUFFER_SIZE-1);
			cout << boolalpha << tree.search(Entry(key, data)) << endl;
			//cout << "Success searching!" << endl;
		}
		else if(!strcmp(commandBuffer, "remove"))
		{
			cin >> key;
			cin.ignore();
			cin.getline(data, DATA_BUFFER_SIZE-1);
			cout << boolalpha << tree.remove(Entry(key, data)) << endl;
			//cout << "Success removing!" << endl;
		}
		else if(!strcmp(commandBuffer, "removeall"))
		{
			cin >> key;
			cout << tree.removeAll(key) << endl;
			//cout << "Success removing all!" << endl;
		}	
	}
}