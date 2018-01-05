#pragma once
#include "String.h"
#include "ctime"

class Tab
{
public:
	Tab();
	Tab(String &_url);

	void printTab() const;
	void setUrl(String &_url);
	String getUrl() const;
	time_t getTimestamp() const;

private:
	String url;
	time_t timeOfOpening;
};

