#pragma once
#include "stdafx.h"
#include "iostream"

using namespace std;

class String
{
		char* data;

		void setData(char* _data);
		int compare(String const& other) const;
public:
		String();
		String(char* data);
		String(String const& other);
		String& operator=(String const& other);
		~String();
 
		size_t size() const;

		char at(size_t idx);
 		char& operator[](size_t idx);

		String& operator+=(String const& rhs);

		bool operator<(const String& rhs) const;
		bool operator>(const String& rhs) const;
		bool operator<=(const String& rhs) const;
		bool operator>=(const String& rhs) const;
		bool operator==(const String& rhs) const;
		bool operator!=(const String& rhs) const;

		friend ostream& operator<<(ostream& os, const String& obj);
		friend istream& operator>>(istream& is, String& obj);

		friend String operator+(String lhs, const String& rhs); 
		const char operator[](size_t idx) const;	
		void split(char symbol, String &secondPart);
}; 