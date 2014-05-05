#pragma once
#include <iostream>
using namespace std;
/*
Class: IOUtil
Purpose: Handles Input/Output functions in obtaining data and displaying information
	to the user.  Incorporates error checking based on the type being returned and in
	certain scenarios, based off of inputs to the function (e.g. getInt implements a
	min/max obtained on function call).


Author: Tyrone Lagore
Version: April 21, 2014

*/
#define MAX_STR_LEN 50
class IOutil
{
public:
	IOutil();
	~IOutil();

	void print(const char *preFormat, const char *message, const char *postFormat);
	void println(const char *format, const char *message, const char *postFormat);
	char getChar(const char *prompt);
	int getInt(const char *prompt, int min, int max);
	bool getYesNo(const char *prompt);
	double getDouble(const char *prompt, double min, double max);
	bool strEqlsIgnoreCase(const char *str1, const char *str2);
	void getString(const char *prompt, char str[], int maxlen, int minlen);
	void waitForKey(const char *prompt);

};

