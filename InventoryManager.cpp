#include "stdafx.h"

#include <cstring>
#include <fstream>
#include <Windows.h>
using namespace std;

#include "InventoryManager.h"
/*
Class name: InventoryManager
Purpose: InventoryManager controls the adding, removal, editting, viewing, saving, and loading
	of anything to do with the system.  All dynamic memory associated with the system, aside
	from the instantiation of the manager itself, is allocated and returned within the 
	InventoryManager.

*/

//Offsets for proper display alignment
#define RIGHT_ALIGN_OFFSET			4
#define ID_FORMAT_OFFSET			7
#define DOLLAR_FORMAT_OFFSET		8
#define QUANTITY_FORMAT_OFFSET		8
#define COST_FORMAT_OFFSET			12
#define QUANTITY_NUMBER_OFFSET		12
#define TOTAL_VALUE_OFFSET			17

#define NAME 'N'
#define COST 'C'
#define QUANTITY 'Q'
#define EXIT 'E'

#define MAX_COST 99999
#define MAX_QUANTITY 9999

InventoryManager::InventoryManager()
{
	m_ConsoleCom = IOutil();
	m_IDCount = 0;
	for (int i = 0; i < INVENTORY_SIZE; i++)
		m_Inventory[i] = NULL;
}

/*
Name: displayGeneralReport
Purpose: Displays an overall inventory count and an overall inventory value.
*/
void InventoryManager::displayGeneralReport()
{
	double dDollarSum = 0;
	int iTotalItems = 0;
	ItemPtr temp;

	for (unsigned int i = 0; i < INVENTORY_SIZE; i++)
	{
		if (m_Inventory[i] != NULL)
		{
			temp = m_Inventory[i];
			while (temp != NULL)
			{
				dDollarSum += (m_Inventory[i]->m_Cost * m_Inventory[i]->m_Quantity);
				iTotalItems += m_Inventory[i]->m_Quantity;

				temp = temp->m_Next;
			}
		}
	}
	printf("%-*s%-*s\n", MAX_TITLE_LEN, "Total Items", MAX_TITLE_LEN, "Gross Value($)");
	printf("%*d%*.2f\n", MAX_TITLE_LEN - RIGHT_ALIGN_OFFSET, iTotalItems, MAX_TITLE_LEN + 3, dDollarSum);
	m_ConsoleCom.waitForKey("Press any key to return to main menu.");
}

/*
Name: displayItem
Purpose: Allows for the looking up and displaying of a single item.
*/
void InventoryManager::displayItem()
{
	ItemPtr oToDisplay = lookupItem();
	if (oToDisplay != NULL)
	{
		displayHeader();
		displaySingleItem(oToDisplay);
	}
	m_ConsoleCom.waitForKey("Press any key to return to main menu.");
}

/*
Name: displayAllItems
Purpose: Displays the details of every item to the screen.

*/
void InventoryManager::displayAllItems()
{
	ItemPtr temp;

	displayHeader();

	for (unsigned int i = 0; i < INVENTORY_SIZE; i++)
	{
		if (m_Inventory[i] != NULL)
		{
			temp = m_Inventory[i];
			while (temp != NULL)
			{
				displaySingleItem(temp);
				temp = temp->m_Next;
			}
		}
	}
	m_ConsoleCom.waitForKey("End of list. Press any key to return to main menu.");
}

/*
Name: displaySingleItem
Purpose: Displays one inventory item's details in a formatted manner.

*/
void InventoryManager::displaySingleItem(ItemPtr item)
{
	printf("%-*d%-*s%*.2f%*d%*.2f\n", ID_FORMAT_OFFSET, item->m_ItemID, MAX_TITLE_LEN + 1,
		item->m_ItemName, DOLLAR_FORMAT_OFFSET, item->m_Cost,
		QUANTITY_NUMBER_OFFSET, item->m_Quantity, TOTAL_VALUE_OFFSET, 
		(double)(item->m_Quantity * item->m_Cost));
}

/*
Name displayHeader
Purpose: Displays the header for when listing all items or a singular item
*/
void InventoryManager::displayHeader()
{
	printf("%-*s%-*s%-*s%-s%-s\n", ID_FORMAT_OFFSET, "ID", MAX_TITLE_LEN + 1, "Name",
		COST_FORMAT_OFFSET, "Price($)", "Quantity", "      Total Value");
}

/*
Name: editItem
Purpose: Allows for the editting of a single item.  The user enters the ID of
	the number in question and is then allowed to edit the cost and quantity of
	the item in stock.
*/
void InventoryManager::editItem()
{
	ItemPtr oToEdit = lookupItem();
	char cUserChoice;

	m_ConsoleCom.println(" ** ", "Edit Item", " ** ");

	if (oToEdit != NULL)
	{
		displayHeader();
		displaySingleItem(oToEdit);
		do
		{
			cUserChoice = toupper(m_ConsoleCom.getChar("Edit [c]ost, [q]uantity, or [e]xit: "));
			switch (cUserChoice)
			{
			case COST:
				oToEdit->m_Cost = m_ConsoleCom.getDouble("Enter new cost: ", 0, MAX_COST);
				break;
			case QUANTITY:
				oToEdit->m_Quantity = m_ConsoleCom.getInt("Enter new quantity: ", 0, MAX_QUANTITY);
				break;
			case EXIT:
				break;
			default:
				m_ConsoleCom.println(" --> ", "Invalid choice", "");
				break;
			}
		} while (cUserChoice != EXIT);

		displayHeader();
		displaySingleItem(oToEdit);

		m_ConsoleCom.waitForKey("Press any key to return to main menu.");
	}

}

/*
Name: AddItem
Purpose: Allows the user to add a new item to the system.  
*/
bool InventoryManager::addItem()
{
	bool bItemAdded = false;
	ItemPtr toAdd;
	m_ConsoleCom.println(" ** ", "Add Item", " ** ");

	toAdd = getItemDetails();
	if (toAdd != NULL)
	{
		linkItem(toAdd);
		bItemAdded = true;
	}

	m_ConsoleCom.waitForKey(bItemAdded ? "Item added.  Press any key to continue." : "Item not added.  Press any key to continue.");

	return bItemAdded;
}

/*
Name: getItemDetails
Purpose: Gets the item details for an individual item being added to the system,
	calls create item to make this new item, then returns the item to the caller.
*/
InventoryManager::ItemPtr InventoryManager::getItemDetails()
{
	ItemPtr oItemToMake = NULL;
	int iID, iQuantity;
	double dCost;
	char sName[MAX_TITLE_LEN + 1];

	m_ConsoleCom.getString("Item name: ", sName, MAX_TITLE_LEN, 1);
	while (!m_ConsoleCom.strEqlsIgnoreCase(sName, "exit") && isNameConflict(sName))
	{
		m_ConsoleCom.println(" --> ", " Name conflict, enter new name or type \"exit\" to cancel", "");
		m_ConsoleCom.getString("Item name: ", sName, MAX_TITLE_LEN, 1);
	}

	if (m_ConsoleCom.strEqlsIgnoreCase(sName, "exit"))
		m_ConsoleCom.println(" --> ", "Canceled operation.", "");
	else
	{
		dCost = m_ConsoleCom.getDouble("Item cost: ", 0, MAX_COST);
		iQuantity = m_ConsoleCom.getInt("Item quantity: ", 0, MAX_QUANTITY);
		iID = generateUniqueID();

		oItemToMake = createItem(sName, dCost, iQuantity, iID);
	}
	return oItemToMake;
}

/*
Name: linkItem
Purpose: Links an item into the system based on it's ID.  The ID (% INVENTORY SIZE)
	is the direct mapping to it's location within the Inventory array.  Each item
	has a next and previous to allow for linear probing.  Instead of going to the 
	next position in the array, however, the item is linked onto the item at the 
	position that it was mapped to.  This ensures that the max speed it takes to 
	find or link an item is O(1 + (numItems / INVENTORY_SIZE)) -> note integer divided
*/
void InventoryManager::linkItem(ItemPtr item)
{
	int IDKey;
	ItemPtr temp;

	IDKey = item->m_ItemID & (INVENTORY_SIZE - 1);

	if (m_Inventory[IDKey] == NULL)
		m_Inventory[IDKey] = item;
	else
	{
		temp = m_Inventory[IDKey]->m_Next;
		while (temp->m_Next != NULL)
			temp = temp->m_Next;

		temp->m_Next = item;
		item->m_Prev = temp;
	}
}

/*
Name: unlinkItem
Purpose:  Unlinks an item from the system and returns the memory to the heap.
	Resolves any unresolved connections due to removing the item and ensures
	no memory leak.
*/
void InventoryManager::unlinkItem(ItemPtr item)
{
	int iIDKey;

	if (item != NULL)
	{
		iIDKey = item->m_ItemID & (INVENTORY_SIZE - 1);

		if (item->m_Next == NULL && item->m_Prev == NULL)
		{
			m_Inventory[iIDKey] = NULL;			//if both previous and next are null, it was the only item at this location
			delete item;
		}
		else if (item->m_Next == NULL)
		{
			item->m_Prev->m_Next = NULL;		// if next is null, it was the last link in the chain at this location
			delete item;						// pointer is resolved, item is deleted.
		}
		else
		{
			item->m_Prev->m_Next = item->m_Next;	// if neither are null, it is in between two items at a location,
			item->m_Next->m_Prev = item->m_Prev;	// connections are resolved, item is deleted.
			delete item;
		}
	}
}

/*
Name: removeItem
Purpose: Allows the user to remove an item from the system.  If the item is 
	found, a confirmation is requested from the user.  If the user confirms the
	remove, it is unlinked and deleted from the system.
*/
bool InventoryManager::removeItem()
{
	bool bRemoved = false;
	ItemPtr oToRemove = lookupItem();

	if (oToRemove != NULL)
	{
		displayHeader();
		displaySingleItem(oToRemove);
		bRemoved = m_ConsoleCom.getYesNo("Are you sure you want to remove this item? (y/n):  ");

		if (bRemoved)
			unlinkItem(oToRemove);
	}

	return bRemoved;
}

/*
Name: lookupItem
Purpose: Looks up and returns an item from the system based on the ID of the item.
*/
InventoryManager::ItemPtr InventoryManager::lookupItem()
{
	int iID, iIDKey;
	ItemPtr oToRemove = NULL;

	do
	{
		iID = m_ConsoleCom.getInt("Enter the ID of the item (-1 to exit)): ", -1, ID_SEED + (m_IDCount));
		if (iID != -1)
		{
			iIDKey = iID & (INVENTORY_SIZE - 1);
			oToRemove = m_Inventory[iIDKey];

			while (oToRemove != NULL && oToRemove->m_ItemID != iID)
				oToRemove = oToRemove->m_Next;

			if (oToRemove == NULL && iID != -1)
				m_ConsoleCom.println(" ** ", "Item not found.", " ** ");
		}
	} while (oToRemove == NULL && iID != -1);

	return oToRemove;
}

/*
Name: isNameConflict
Purpose: Checks to see if the name handed in conflicts with any of the names
	currently in the system, ignoring case.  If a conflict is found, true is
	returned, else false.
*/
bool InventoryManager::isNameConflict(char *name)
{
	bool bConflict = false;
	bool bNoCurrentConflict = true;
	ItemPtr oTemp;

	for (unsigned int i = 0; i < INVENTORY_SIZE && !bConflict; i++)
	{
		oTemp = m_Inventory[i];
		while (oTemp != NULL && !bConflict)
		{
			if (m_ConsoleCom.strEqlsIgnoreCase(name, oTemp->m_ItemName))
				bConflict = true;

			oTemp = oTemp->m_Next;
		}
	}

	return bConflict;
}

/*
Name: generateUniqueID
Purpose: Generates a unique ID for the item based on an ID seed and the number of
	items that have been added to the system.
*/
int InventoryManager::generateUniqueID()
{
	int iID = ID_SEED + m_IDCount;
	m_IDCount++;
	return iID;
}

/*
Name: createItem
Purpose: Creates and returns an item given a name, cost, quantity, and ID.
	The item is not linked into the actual system within this function.
*/
InventoryManager::ItemPtr InventoryManager::createItem(char *name, double cost, int quantity, int ID)
{
	ItemPtr oToAdd = new Item;
	strcpy_s(oToAdd->m_ItemName, name);
	oToAdd->m_Cost = cost;
	oToAdd->m_ItemID = ID;
	oToAdd->m_Quantity = quantity;
	oToAdd->m_Next = NULL;
	oToAdd->m_Prev = NULL;
	return oToAdd;
}

/*
Name: saveToFile
Purpose: Attempts to save the file to the specified filename.  If the file is successfully
	opened and written to, the caller is returned true, otherwise false is returned.
*/
bool InventoryManager::saveToFile(char *fileName)
{
	ofstream fOutFile;
	bool bOpened = false;
	ItemPtr oTemp;

	fOutFile.open(fileName);

	if (!fOutFile.fail())
	{
		for (unsigned int i = 0; i < INVENTORY_SIZE; i++)
		{
			if (m_Inventory[i] != NULL)
			{
				oTemp = m_Inventory[i];
				while (oTemp != NULL)
				{
					fOutFile << oTemp->m_Quantity << " " << oTemp->m_Cost << " " << oTemp->m_ItemName << endl;
					oTemp = oTemp->m_Next;
				}
			}
		}
		bOpened = true;
		fOutFile.close();
	}

	return bOpened;
}

/*
Name: loadFromFile
Purpose: Attempts to open and load a files information.  If the file is opened,
	the data is read in and assumed to be in formatted fashion.

Assumptions:
	The file being read in was written by this program, or the writer of the file
	has formatted the file being read in the manner that it would have been written
	by the program.
*/
bool InventoryManager::loadFromFile(char *fileName)
{
	bool bOpened = false;
	ifstream fInfile;
	char sName[MAX_TITLE_LEN + 1],
		cSpace;
	int iQuantity, iID;
	double dCost;
	ItemPtr temp;

	fInfile.open(fileName);

	if (!fInfile.fail())
	{
		fInfile >> iQuantity;
		while (!fInfile.eof())
		{
			fInfile.get(cSpace);
			fInfile >> dCost;
			fInfile.get(cSpace);
			fInfile.getline(sName, MAX_TITLE_LEN);

			iID = generateUniqueID();

			temp = createItem(sName, dCost, iQuantity, iID);
			linkItem(temp);
			fInfile >> iQuantity;
		}
		fInfile.close();
		bOpened = true;
	}

	return bOpened;
}

/*
Deconstructor
Purpose: Returns any dynamic memory used to the heap
*/
InventoryManager::~InventoryManager()
{
	ItemPtr oTemp, oToDelete;
	for (unsigned int i = 0; i < INVENTORY_SIZE; i++)
	{
		if (m_Inventory[i] != NULL)
		{
			oTemp = m_Inventory[i];
			while (oTemp != NULL)
			{
				oToDelete = oTemp;
				oTemp = oTemp->m_Next;
				unlinkItem(oToDelete);
			}
		}
	}
}

