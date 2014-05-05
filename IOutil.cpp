#include "stdafx.h"

#include "IOutil.h"
#include <conio.h>

IOutil::IOutil(){}


void IOutil::print(const char* preFormat, const char* message, const char* postFormat)
{
	cout << preFormat << message << postFormat;
}


/*

*/
void IOutil::println(const char* preFormat, const char* message, const char* postFormat)
{
	cout << preFormat << message << postFormat << endl;
}

/*
Name: getChar
Purpose: Gets a character from the user.  Reprompts if the user if they
	do not enter exactly 1 character followed by the return key.
*/
char IOutil::getChar(const char *prompt)
{
	char cInput,cDelimKill = '\0';

	print("", prompt, "");

	cin.get(cInput);

	if (cInput != '\n')
		cin.get(cDelimKill);			// clear new line from buffer

	while (cInput == '\n' || cDelimKill != '\n')
	{
		if (cDelimKill != '\n' && cInput != '\n')
			cin.ignore(1024, '\n');		// clear buffer on more than 1 key entered

		print("Invalid entry.\n", prompt, "");
		cin.get(cInput);

		if (cInput != '\n')
			cin.get(cDelimKill);
	}

	return cInput;
}

/*
Name: getInt
Purpose: Gets an integer between a specified minimum value and maximum value
	from the user.  Incorporates error checking for malformed inputs (not integers)
	and invalid inputs.

Known bugs:
	Due to cin ignoring whitespace, if the user enters a bunch of spaces then a
	valid integer, the integer will be accepted.
*/
int IOutil::getInt(const char *prompt, int min, int max)
{
	int iInput;
	char cDelimKill = '\0';

	print("", prompt, "");

	cin >> iInput;

	if (!cin.fail())
		cin.get(cDelimKill);		//clear new line

	while (cin.fail() || iInput < min || iInput > max || cDelimKill != '\n')
	{
		if (cin.fail())
			cin.clear();		//if bad entry, clear state

		if (cDelimKill != '\n')
			cin.ignore(1024, '\n');

		cout << "Invalid entry.  Number must be between " << min << " and " << max << ".\n" << prompt;
		cin >> iInput;

		if (!cin.fail())
			cin.get(cDelimKill);
	}

	return iInput;
}

/*
Name: getYesNo
Purpose: Gets a 'y'es or 'n'o from the user and returns the result
	as a boolean answer. (true for 'y' and false for 'n')
*/
bool IOutil::getYesNo(const char *prompt)
{
	bool bInput;
	char cUserInput;

	cUserInput = toupper(getChar(prompt));

	while (cUserInput != 'Y' && cUserInput != 'N')
	{
		print("", "Invalid entry.\n", "");
		cUserInput = getChar(prompt);
		toupper(cUserInput);
	}

	bInput = cUserInput == 'Y' ? true : false;
	return bInput;
}

/*
Name: getInt
Purpose: Gets a double between a specified minimum value and maximum value
	from the user.  Incorporates error checking for malformed inputs (not doubles)
	and invalid inputs.

Known bugs:
	Same as getInt
*/
double IOutil::getDouble(const char *prompt, double min, double max)
{
	double iInput;
	char cDelimKill = '\0';

	print("", prompt, "");

	cin >> iInput;

	if (!cin.fail())
		cin.get(cDelimKill);		//clear new line

	while (cin.fail() || iInput < min || iInput > max || cDelimKill != '\n')
	{
		if (cin.fail())
			cin.clear();		//if bad entry, clear state

		if (cDelimKill != '\n')
			cin.ignore(1024, '\n');

		cout << "Invalid entry.  Number must be between " << min << " and " << max << ".\n" << prompt;
		cin >> iInput;

		if (!cin.fail())
			cin.get(cDelimKill);
	}

	return iInput;
}

/*
Name: strEqlsIgnoreCase
Purpose: Checks to see if two strings are equal, ignoring case.

Method: First checks if the strings are of same length.  If they are,
	it checks character by character (forcing the character to upper
	to ensure case insensitivity). If any discrepancy is found, the
	strings are not equal.

Assumptions: Both strings do not have trailing whitespace. If they do,
	a match will not be caught if the strings differ in length.
*/
bool IOutil::strEqlsIgnoreCase(const char *str1, const char *str2)
{
	bool bIsEqual = true;

	if (strlen(str1) == strlen(str2))
	{
		for (int i = 0; i < strlen(str1) && bIsEqual; i++)
			if ((char)toupper(str1[i]) != (char)toupper(str2[i]))
				bIsEqual = false;
	}else
		bIsEqual = false;

	return bIsEqual;
}
/*
Name: getString
Purpose: Obtains a string from the user with greater than specified minimum length
	and shorter than specified max length.
*/
void IOutil::getString(const char *prompt, char str[], int maxlen, int minlen)
{
	print("", prompt, "");
	cin.getline(str, maxlen + 1, '\n');

	while (cin.fail() || strlen(str) < minlen)
	{
		cin.clear();

		if (strlen(str) >= maxlen)
			cin.ignore(1024, '\n');

		cout << "Invalid entry.  Please enter a string between " << minlen << " and " << maxlen << " characters." << endl;
		print("", prompt, "");
		cin.getline(str, maxlen + 1, '\n');
	}

	for (int i = strlen(str) - 1; i > 0 && str[i] == ' '; i--)	// Clips any trailing whitespace
		str[i] = '\0';

	cin.clear();

	return;
}

/*
Name: waitForKey
Purpose: Waits for any key to be entered, does not wait for return key to be struck
	and does not echo key to the console.
*/
void IOutil::waitForKey(const char *prompt)
{
	print(" --> ", prompt, " <--");
	_getch();
}

IOutil::~IOutil(){}
