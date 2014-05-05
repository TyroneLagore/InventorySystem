#include "stdafx.h"
#include "Menu.h"

#include <iostream>
#include <Windows.h>
using namespace std;

/*
Constructor
Inputs:
	const char menuName[] - The name of the new menu 

*/
Menu::Menu( const char menuName[] )		
{ 
	unsigned int i;
	m_NumOptions = 0; 
	for (i = 0; i < strlen(menuName) && i < MAX_STR_LEN - 1; i++)
		m_MenuName[i] = menuName[i];

	m_MenuName[i] = '\0';
}

/*
Name: addOption
Purpose: Adds an option to the available menu selections.
Inputs:
	const char* optionName - The display name of the option to add
	const char option - The key assigned to this option

Restrictions:
	Option name character length cannot exceed 40 characters 

	Number of options is currently restricted to 15 options.  No error
		message is returned or displayed, the option is simply not added
		after 15 options.
*/
void Menu::addOption(const char* optionName, const char option)
{
	unsigned int i;

	if (m_NumOptions + 1 <= MAX_OPTIONS)
	{
		for (i = 0; i < strlen(optionName) && i < MAX_OPTION_LEN; i++)
			m_Options[m_NumOptions].optionName[i] = optionName[i];
		
		m_Options[m_NumOptions].optionName[i] = '\0';
		m_Options[m_NumOptions].option = option;

		m_NumOptions++;
	}
}

/*
Name: getUserChoice
Purpose: Gets a character from the user and cross references it with available
	choices from the options that have been added to the menu.  If the menu
	has at least 1 option, it will display the menu and wait for the user to 
	ender an option.  Until a valid selection is entered, the menu will continue
	to prompt the user.
*/
char Menu::getUserChoice()
{
	bool bBadChoice = true;
	char cChoice;

	if (m_NumOptions > 0)
	{
		while (bBadChoice)
		{
			displayMenu();
			cChoice = m_ConsoleCom.getChar("Choice: ");

			for (int i = 0; i < m_NumOptions && bBadChoice; i++)
				bBadChoice = m_Options[i].option != toupper(cChoice);

			if (bBadChoice)
			{
				m_ConsoleCom.print("", "Invalid Selection.", "");
				for (int i = 0; i < 3; i++)
				{
					Sleep(500);
					m_ConsoleCom.print("", ".", "");
				}
			}
		}
	}

	return cChoice;
}

/*
Name: Display Menu
Purpose: Displays the menu in a formatted manner
*/
void Menu::displayMenu()
{
	system("CLS");

	m_ConsoleCom.println (" --- ", m_MenuName, " --- ");

	for (int i = 0; i < m_NumOptions; i++)
		cout << "  (" << m_Options[i].option << ") " << m_Options[i].optionName << endl;
}


Menu::~Menu()
{
}
