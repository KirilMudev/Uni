#pragma once
#include "DLList.h"
#include "Tab.h"

const int FIRST_INDEX = 0;

class TabModule
{
public:
	TabModule();

	void insertTab(String &newTabUrl);
	void loadUrl(String &url);
	void removeCurrentTab();
	void moveForward();
	void moveBack();
	void printTabs();
	void sortTabs(int key);

private:
	DLList<Tab> tabs;
	int currentTabIndex;
};

