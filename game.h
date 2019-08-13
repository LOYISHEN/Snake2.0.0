/*
* filename : game.h
* author : PenG
* create time : 2019/08/12
* version : 19.08.12
*/

#pragma once

#ifndef _GAME_H_
#define _GAME_H_

#include "food.h"
#include "map.h"
#include "print.h"
#include "snake.h"
#include "wall.h"
#include "scanner.h"

namespace game
{
	
	class Game
	{
	public:
		Game(unsigned int mapWidth, unsigned int mapHeight, unsigned int gameSpeed = 50);

		void start();

	private:

		bool turn();

		Map *m_map;
		Printer *m_printer;
		Food *m_food;
		Snake *m_snake;
		Wall *m_wall;
		Scanner *m_scanner;

		int m_gameSpeed;
		int m_mapWidth;
		int m_mapHeight;
	};
}

#endif