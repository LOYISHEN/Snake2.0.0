/*
* filename : scanner.h
* author : PenG
* create time : 2019/08/13
* version : 19.08.13
*/

#pragma once

#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <conio.h>
#include <iostream>
#include <string>

class Scanner
{
public:

	char getChar() const
	{
		char input = 0;
		if (_kbhit())
		{
			input = getch();
			cleanInput();

			return input;
		}
		return 0;
	}

private:

	void cleanInput() const
	{
		while (_kbhit())
		{
			getch();
		}
	}

};

#endif