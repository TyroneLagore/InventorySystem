#pragma once

#include "IOutil.h"

#define MAX_TITLE_LEN 15
#define INVENTORY_SIZE 512
#define ID_SEED 0x1C00

/*
Class: InventoryManager
Purpose: Maintains information pertaining to the inventory system and 
	all functions that act upon the system.  Handles adding, removal, 
	displaying, saving, and loading of the system.  The file IO is currently
	specific to this program and does not contain error checking.  It is
	assumed that the file that is requested to open is a file that has been
	written by this class.  
*/

class InventoryManager
{
public:
	InventoryManager();
	~InventoryManager();
	void displayGeneralReport();
	void displayAllItems();
	void displayItem();
	void editItem();
	bool addItem();
	bool removeItem();
	bool saveToFile(char *fileName);
	bool loadFromFile(char *fileName);

private:
	IOutil m_ConsoleCom;
	int m_IDCount;

	struct Item
	{
		int m_ItemID;
		char m_ItemName[MAX_TITLE_LEN + 1];
		double m_Cost;
		int m_Quantity;
		Item *m_Next;
		Item *m_Prev;
	};

	typedef Item* ItemPtr;

	ItemPtr m_Inventory[INVENTORY_SIZE];

	ItemPtr createItem(char *name, double cost, int quantity, int ID);
	ItemPtr getItemDetails();
	ItemPtr lookupItem();
	void linkItem(ItemPtr item);
	void unlinkItem(ItemPtr item);
	void displaySingleItem(ItemPtr item);
	void displayHeader();
	int generateUniqueID();
	bool isNameConflict(char *name);
};

