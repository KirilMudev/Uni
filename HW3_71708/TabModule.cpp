#include "stdafx.h"
#include "TabModule.h"

TabModule::TabModule()
{
	tabs.append(Tab());
	currentTabIndex = FIRST_INDEX;
}

void TabModule::insertTab(String &newTabUrl)
{
	tabs.insertAfter(currentTabIndex, Tab(newTabUrl));
	currentTabIndex++;
}

void TabModule::loadUrl(String &url)
{
	this->tabs.getAt(currentTabIndex).setUrl(url);
}

void TabModule::removeCurrentTab()
{
	if(this->currentTabIndex != tabs.getSize())
	{
		this->tabs.removeElement(currentTabIndex);
	}
	else if(this->currentTabIndex != FIRST_INDEX)
	{
		this->tabs.removeElement(currentTabIndex);
		this->currentTabIndex--;
	}
	else
	{
		this->tabs.getAt(currentTabIndex).setUrl(String("about:blank"));
	}
}

void TabModule::moveForward()
{
	if(this->currentTabIndex != tabs.getSize())
	{
		currentTabIndex++;
	}
	else
	{
		currentTabIndex = FIRST_INDEX;
	}
}

void TabModule::moveBack()
{
	if (this->currentTabIndex != FIRST_INDEX)
	{
		currentTabIndex--;
	}
	else
	{
		currentTabIndex = tabs.getSize();
	}
}

void TabModule::printTabs()
{
	Iterator<Tab> it = tabs.getIterator(); //from first elem

	int listSize = tabs.getSize();

	for (int i = 0; i <= listSize; i++)
	{
		if (i == currentTabIndex)
		{
			cout << ">";
		}

		it.getCurrent().printTab();
		it.moveToNext();
	}
}

void TabModule::sortTabs(int key)
{
	this->tabs.sort(key);
	this->currentTabIndex = FIRST_INDEX;
}
