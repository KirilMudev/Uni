#pragma once
#include "iostream"

class Entry
{
public:
	Entry() : key(0), data(nullptr) {}
	Entry(int _key, char* _data);
	Entry(const Entry& other);
	Entry& operator=(const Entry& other);
	~Entry();

	void setData(char* _data);
	void setKey(int _key);
	int getKey() const;
	char* getData() const;
	bool operator>(const Entry& other) const;
	bool operator==(const Entry& other) const;
	
private:
	int key;
	char* data;
};

Entry::Entry(int _key, char* _data)
{
	this->setKey(_key);
	this->setData(_data);
}

Entry::Entry(const Entry& other)
{
	this->key = other.key;
	this->setData(other.data);
}

Entry& Entry::operator=(const Entry& other)
{
	if(this != &other)
	{
		delete[] data;
		data = nullptr;
		this->key = other.key;
		this->setData(other.data);
	}

	return *this;
}

Entry::~Entry()
{
	delete[] data;
}

void Entry::setData(char* _data)
{
	this->data = new char[strlen(_data) + 1];
	strcpy(data, _data);
}

void Entry::setKey(int _key)
{
	this->key = _key;
}

int Entry::getKey() const
{
	return this->key;
}

char* Entry::getData() const
{
	return this->data;
}
	
bool Entry::operator>(const Entry& other) const
{
	if(this->key > other.key)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Entry::operator==(const Entry& other) const
{
	if(this->key == other.key && !strcmp(this->data, other.data))
	{
		return true;
	}
	else
	{
		return false;
	}
}