#include "stdafx.h"
#include "Shell.h"

Shell::Shell()
{
	this->tabModule = new TabModule();
	this->isActive = true;
	cout << "Shell initialized: " << endl;
}

Shell::~Shell()
{
	delete tabModule;
}

void Shell::print()
{
	tabModule->printTabs();
}

void Shell::go(String &url)
{
	tabModule->loadUrl(url);
}
	
void Shell::insert(String &url)
{
	tabModule->insertTab(url);
}
void Shell::back()
{
	tabModule->moveBack();
}

void Shell::forward()
{
	tabModule->moveForward();
}

void Shell::remove()
{
	tabModule->removeCurrentTab();
}

void Shell::interpret()
{
	String buffer;
	cin >> buffer;
	String userInput;
	buffer.split(' ', userInput);

	if(buffer == "GO")
	{
		this->go(userInput);
	}
	else if(buffer == "INSERT")
	{
		this->insert(userInput);
	}
	else if(buffer == "REMOVE")
	{
		this->remove();
	}
	else if(buffer == "BACK")
	{
		this->back();
	}
	else if(buffer == "FORWARD")
	{
		this->forward();
	}
	else if(buffer == "PRINT")
	{
		this->print();
	}
	else if(buffer == "SORT")
	{
		if (userInput == "URL")
		{
			this->tabModule->sortTabs(1);
		}
		else if(userInput == "TIME")
		{
			this->tabModule->sortTabs(2);
		}
		else
		{
			cerr << "Invalid argument, you should either use \"URL\" or \"TIME\"." << endl;
		}
	}
	else if (buffer == "HELP")
	{
		this->printHelp();
	}
	else if(buffer == "EXIT")
	{
		this->isActive = false;
		cout << "Shell terminated." << endl;
	}
	else
	{
		cerr << "Invalid argument, for help use \"HELP\" command. To exit, use \"EXIT\"." << endl;
	}
}

void Shell::printHelp()
{
	cout << "Commands you can use: " << endl;
	cout << "GO <url> : loads page in current tab." << endl;
	cout << "INSERT <url> : opens new tab, and loads url in it" << endl;
	cout << "FORWARD : goes to next tab" << endl;
	cout << "BACK : goes to previous tab" << endl;
	cout << "REMOVE : removes current tab" << endl;
	cout << "PRINT : prints all tabs" << endl;
	cout << "SORT <URL> | SORT <TIME> : sorts tabs by given criteria" << endl;
	cout << "EXIT : exits the shell" << endl;
}

void Shell::open()
{
	while (this->isActive == true)
	{
		this->interpret();
	}
}