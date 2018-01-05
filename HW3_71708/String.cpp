#include "stdafx.h"
#include "String.h"

String::String()
{
	data = new char[1];
	strcpy(data, "");
}

String::String(char* _data)
{
	this->setData(_data);
}

String::String(String const& other)
{
	this->setData(other.data);
}

String& String::operator=(String const& other)
{
	if (this != &other)
	{
		this->setData(other.data);
	}

	return *this;
}

String::~String()
{
	delete[] data;
}

void String::setData(char* _data)
{
	data = new char[strlen(_data) + 1];
	strcpy(data, _data);
}

char String::at(size_t idx)
{
	if (idx > this->size() || idx < 0)
	{
		throw invalid_argument("Out of bounds!");
	}

	return this->data[idx];
}

size_t String::size() const
{
	size_t counter = 0;

	while (this->data[counter])
	{
		counter++;
	}

	return counter; 
}

char& String::operator[](size_t idx)
{
	if (idx > this->size() || idx < 0)
	{
		throw invalid_argument("Out of bounds!");
	}

	return this->data[idx];
}

const char String::operator[](size_t idx) const
{
	if (idx > this->size() || idx < 0)
	{
		throw invalid_argument("Out of bounds!");
	}

	return this->data[idx];
}

String operator+(String lhs, const String& rhs)
{
	lhs += rhs;
	return String(lhs);
}

String& String::operator+=(String const& _newStr)
{
	size_t newLen = this->size() + _newStr.size() + 1;
	char* tempData = new char[newLen];

	for (size_t i = 0; i < newLen; i++)
	{
		if (i < this->size())
		{
			tempData[i] = this->data[i];
		}
		else
		{
			tempData[i] = _newStr[i-this->size()];
		}
	}

	
	delete[] this->data;
	this->data = tempData;

	return *this;
} 

int String::compare(String const& other) const //my version of strcmp
{
    int minSize = (this->size() < other.size() ? this->size() : other.size());

    for (int i = 0; i < minSize; i++) 
	{
        if (this->data[i] < other.data[i])
		{
            return -1;
		}

        if (this->data[i] > other.data[i])
		{
            return 1;
		}
    }

    if (this->size() != other.size())
	{
        return (this->size() < other.size()) ? -1 : 1;
	}

    return 0;
}

bool String::operator<(const String& other) const
{
    return this->compare(other) < 0;
}

bool String::operator>(const String& other) const 
{
    return this->compare(other) > 0;
}

bool String::operator<=(const String& other) const
{
    return this->compare(other) <= 0;
}

bool String::operator>=(const String& other) const 
{
    return this->compare(other) >= 0;
}

bool String::operator==(const String& other) const 
{
    return this->compare(other) == 0;
}

bool String::operator!=(const String& other) const
{
    return this->compare(other) != 0;
}

ostream& operator<<(ostream& os, const String& obj)
{
	os << obj.data;
	return os;
}

istream& operator>>(istream& is, String& obj)
{
	char buffer[1024];
	is.getline(buffer, 1024);
	obj.setData(buffer);
	return is;
}

void String::split(char symbol, String &secondPart)
{
	int strSize = this->size();
	int position = 0;
	while (this->data[position] != symbol)
	{
		if(position > strSize)
		{
			return;
		}

		position++;
	}

	char* leftSide = new char[position+1];
	for (int i = 0; i < position; i++)
	{
		leftSide[i] = this->data[i];
	}
	leftSide[position] = '\0';

	char* rightSide = new char[strSize-position];
	for (int i = position; i < strSize; i++)
	{
		rightSide[i-position] = this->data[i+1]; 
	}

	delete[] this->data;
	this->setData(leftSide);
	delete[] leftSide;

	secondPart.setData(rightSide);
	delete[] rightSide;
}