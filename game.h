/*
* filename : game.h
* author : PenG
* create time : 2019/08/12
* version : 19.08.12
*/

#pragma once

#ifndef _GAME_H_
#define _GAME_H_

#include <conio.h>
#include <iostream>
#include <string>

namespace game {
	class Scanner
	{
	public:
		
		char getChar()
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

		void cleanInput()
		{
			while (_kbhit())
			{
				getch();
			}
		}

	};

}

#endif