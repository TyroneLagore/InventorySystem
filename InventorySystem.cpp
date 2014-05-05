// InventorySystem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IOutil.h"
#include "Menu.h"
#include "InventoryManager.h"

#define MAX_FILE_SIZE 20

#define VIEW_GENERAL	'1'
#define VIEW_ALL		'2'
#define VIEW_ITEM		'3'
#define ADD_ITEM		'4'
#define	EDIT_ITEM		'5'
#define	REMOVE_ITEM		'6'
#define	SAVE_OR_LOAD	'7'
#define QUIT			'Q'
#define SAVE			'S'
#define LOAD			'L'
#define CANCEL			'C'

void initializeMenu(Menu *menu);
void run(Menu *menu, InventoryManager *manager);
void saveOrLoad(InventoryManager *manager);

int _tmain(int argc, _TCHAR* argv[])
{
	InventoryManager *m_InventoryManager = new InventoryManager();
	Menu menu( "InvTrack v1.0" );
	initializeMenu(&menu);
	run( &menu, m_InventoryManager );
	delete m_InventoryManager;
	return 0;
}

/*
Name: initializeMenu
Purpose: Creates the menu items and options that will be accessible 
	via the menu class.
*/
void initializeMenu(Menu *menu)
{
	menu->addOption("View General Overall", '1');
	menu->addOption("List All Items", '2');
	menu->addOption("View an Item", '3');
	menu->addOption("Add an Item", '4');
	menu->addOption("Edit an Item", '5');
	menu->addOption("Remove an Item", '6');
	menu->addOption("Save To or Load From File", '7');
	menu->addOption("Quit", 'Q');
}

/*
Name: run
Purpose: Runs the Inventory system and menu loop until the user selects 
	the quit option.

*/
void run(Menu *menu, InventoryManager *manager)
{
	bool bQuit = false;
	do
	{
		switch (toupper(menu->getUserChoice()))
		{
		case VIEW_GENERAL:
			manager->displayGeneralReport();
			break;
		case VIEW_ALL:
			manager->displayAllItems();
			break;
		case VIEW_ITEM:
			manager->displayItem();
			break;
		case ADD_ITEM:
			manager->addItem();
			break;
		case EDIT_ITEM:
			manager->editItem();
			break;
		case REMOVE_ITEM:
			manager->removeItem();
			break;
		case SAVE_OR_LOAD:
			saveOrLoad(manager);
			break;
		case QUIT:
			bQuit = true;
			break;
		};
	} while (!bQuit);
}

void saveOrLoad(InventoryManager *manager)
{
	IOutil consoleCom = IOutil();
	char sFileName[MAX_FILE_SIZE], cSaveOrLoad;
	bool bExit = false;

	do
	{
		cSaveOrLoad = toupper(consoleCom.getChar("Would you like to (s)ave, (l)oad, or (c)ancel?: "));
		if (cSaveOrLoad == SAVE || cSaveOrLoad == LOAD)
		{
			consoleCom.getString("Enter file name: ", sFileName, MAX_FILE_SIZE, 1);
			if (cSaveOrLoad == SAVE ? manager->saveToFile(sFileName) : manager->loadFromFile(sFileName))
			{
				consoleCom.println(" --> ", "Success.", " <-- ");
				bExit = true;
			}
			else
				consoleCom.println(" --> ", "Failed to open file.", " <-- ");
		}
		else if (cSaveOrLoad == CANCEL)
			bExit = true;
		else
			consoleCom.println(" --> ", "Invalid entry.", "");
	} while (!bExit);

	consoleCom.waitForKey("Press any key to return to the main menu.");
}