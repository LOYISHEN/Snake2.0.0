/*
* filename : game.h
* author : PenG
* create time : 2019/08/13
* version : 19.08.13
*/

#include "game.h"
using namespace game;

Game::Game(unsigned int mapWidth, unsigned int mapHeight, unsigned int gameSpeed)
{
	if (mapWidth < 20 || mapHeight < 20)
	{
		mapWidth = 20;
		mapHeight = 20;
	}

	m_mapWidth = mapWidth;
	m_mapHeight = mapHeight;

	m_map = new Map(mapWidth, mapHeight);
	m_printer = new Printer(m_map);
	m_food = new Food(m_map);
	m_snake = new Snake(m_map);
	m_wall = new Wall(m_map);
	m_scanner = new Scanner();

	if (gameSpeed < 50)
	{
		m_gameSpeed = 50;
	}
	else
	{
		m_gameSpeed = gameSpeed;
	}

}

void Game::start()
{
	m_wall->printToMap();
	m_food->set();

	bool pause = false;
	
	while (true)
	{
		
		if (m_snake->isEatFood())
		{
			m_snake->eatFood();
			m_food->set();
		}
		else
		{
			m_snake->move();
		}

		m_wall->printToMap();

		if (m_snake->isBiteSelf() || m_snake->isHitWall())
		{
			Sleep(1000);

			m_map->init(m_mapWidth, m_mapHeight);
			m_snake->init();
			m_wall->printToMap();
			m_food->set();

			m_printer->printMap();

			while (turn());

			m_scanner->getChar();

			continue;
		}
		m_printer->printMap();
		
		Sleep(m_gameSpeed);

		pause = !turn();
		while (pause)
		{
			pause = turn();
		}
		
	}
}

bool Game::turn()
{
	char input;
	if ((input = m_scanner->getChar()) != 0)
	{
		switch (input)
		{
		case 'w':
		case 'W':
			m_snake->turn(DIRECTION_UP);
			break;

		case 'd':
		case 'D':
			m_snake->turn(DIRECTION_RIGHT);
			break;

		case 's':
		case 'S':
			m_snake->turn(DIRECTION_DOWN);
			break;

		case 'a':
		case 'A':
			m_snake->turn(DIRECTION_LEFT);
			break;

		case 'f':
		case 'F':
			return false;
			break;

		default:
			break;
		}
	}
	return true;
}