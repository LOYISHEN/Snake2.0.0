/*
* filename : main.cpp
* author : PenG
* create time : 2019/08/12
* version : 19.08.12
*/

#define _CRT_SECURE_NO_WARNINGS
#include "food.h"
#include "map.h"
#include "print.h"
#include "snake.h"
#include "wall.h"
#include "game.h"
#include <iostream>

using namespace std;

int main()
{
	game::Scanner scanner;

	Map map;
	map.init(20, 20);

	Snake snake(&map);
	Printer printer(&map);
	Wall wall(&map);
	wall.printToMap();
	
	char debugMessage[512] = { 0 };

	char input;

	while (true)
	{

		if (snake.isEatFood())
		{
			snake.eatFood();
		}
		else
		{
			snake.move();
		}

		wall.printToMap();
		
		if (snake.isBiteSelf() || snake.isHitWall())
		{
			cout << "over" << endl;
			Sleep(1000);
			break;
		}

		sprintf(debugMessage, "bite self is : %d , hit wall is : %d , ", snake.isBiteSelf(), snake.isHitWall());
		OutputDebugString(debugMessage);
		
		Sleep(200);

		printer.printMap();
		
		if ((input = scanner.getChar()) != 0)
		{
			sprintf(debugMessage, "turn : %c ", input);
			OutputDebugString(debugMessage);
			switch (input)
			{
			case 'w':
				snake.turn(DIRECTION_UP);
				break;

			case 'd':
				snake.turn(DIRECTION_RIGHT);
				break;

			case 's':
				snake.turn(DIRECTION_DOWN);
				break;

			case 'a':
				snake.turn(DIRECTION_LEFT);
				break;

			default:
				break;
			}
		}
	}
	system("pause");
	return 0;
}