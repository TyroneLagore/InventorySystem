#pragma once

#include "IOutil.h"

#define MAX_OPTIONS 15
#define MAX_OPTION_LEN 40
/*
Class: Menu
Purpose: Generalizes a basic menu console prompt system allowing the user
	to add their own options and option choice (a character).  The menu
	also incorporates error checking functionality to ensure that when
	the user choice is requested, that the option returned is within
	the menu system.

*/
class Menu
{
public:
	Menu(const char menuName[]);
	~Menu();

	char getUserChoice();
	void addOption(const char* optionName, const char option);

private:
	IOutil m_ConsoleCom;
	int m_NumOptions;
	char m_MenuName[MAX_STR_LEN + 1];

	struct Option{
		char optionName[MAX_OPTION_LEN + 1];
		char option;
	};
	Option m_Options[MAX_OPTIONS];

	void displayMenu();
	
};

