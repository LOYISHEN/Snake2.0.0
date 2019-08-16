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
	if (mapWidth < 15 || mapHeight < 15)
	{
		mapWidth = 15;
		mapHeight = 15;
	}

	m_mapWidth = mapWidth;
	m_mapHeight = mapHeight;

	m_map = new Map(mapWidth, mapHeight);
	m_printer = new Printer(m_map);
	m_food = new Food(m_map);
	m_snake = new Snake(m_map);
	m_wall = new Wall(m_map);
	m_scanner = new Scanner();

	if (gameSpeed < 1)
	{
		m_gameSpeed = 1;
	}
	else
	{
		m_gameSpeed = gameSpeed;
	}

	m_snakeAI = new SnakeAI(m_map, m_snake, m_food);
}

void Game::start()
{
	m_wall->printToMap();
	m_food->set();

	bool pause = false;
	int counter = 0;
	int lastLength = 3;
	
	while (true)
	{
		char direction;
		if ((direction = m_snakeAI->getDirection()) == -1)
		{
			throw "error";
			return;
		}
		m_snake->turn(direction);
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

		m_printer->printMap();

		if (m_snake->isBiteSelf() || m_snake->isHitWall())
		{
			Sleep(1000);

			initGameEnvironment();

			//while (turn());

			//m_scanner->getChar();

			continue;
		}
		
		Sleep(m_gameSpeed);

		//pause = !turn();
		//while (pause)
		//{
		//	pause = turn();
		//}

		if (lastLength == m_snake->getLength())
		{
			counter++;
			if (counter > 5000)    //死循环吃不了食物了
			{
				counter = 0;

				char message[128] = { 0 };
				sprintf(message, "snake length is : %d\n", m_snake->getLength());
				OutputDebugString(message);

				Sleep(1000);
				initGameEnvironment();
				lastLength = m_snake->getLength();
			}
		}
		else
		{
			lastLength = m_snake->getLength();
			counter = 0;
		}

		if (m_snake->getLength() == m_map->getSize() - 2 * m_mapHeight - 2 * m_mapWidth + 4)
		{
			return;
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

void Game::initGameEnvironment()
{
	m_map->init(m_mapWidth, m_mapHeight);
	m_snake->init();
	m_wall->printToMap();
	m_food->set();

	m_printer->printMap();
}