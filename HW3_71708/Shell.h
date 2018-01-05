#pragma once
#include "TabModule.h"
#include "String.h"

class Shell
{
public:
	Shell();
	~Shell();

	void open();

private:
	TabModule * tabModule;
	bool isActive;

	void go(String &url); 
	void insert(String &url); 
	void back(); 
	void forward(); 
	void remove(); 
	void print(); 
	void printHelp();
	void interpret();
};
