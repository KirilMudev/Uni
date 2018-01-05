#include "stdafx.h"
#include "Tab.h"

Tab::Tab()
{
	this->url = "about:blank";
	time(&timeOfOpening);
}

Tab::Tab(String &_url)
{
	this->url = _url;
	time(&timeOfOpening);
}

void Tab::printTab() const
{
	cout << this->url << " " << this->timeOfOpening << '\n';
}

void Tab::setUrl(String &_url)
{
	this->url = _url;
	time(&timeOfOpening);
}

String Tab::getUrl() const
{
	return this->url;
}

time_t Tab::getTimestamp() const
{
	return this->timeOfOpening;
}