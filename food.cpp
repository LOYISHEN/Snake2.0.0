/*
* filename : food.cpp
* author : PenG
* create time : 2019/07/31
* version : 19.07.31
*/

#include "food.h"

void Food::set()
{
	srand(time(NULL));
	
	int mapWidth = m_map->getWidth();
	int mapHeight = m_map->getHeight();

	int x = 0;
	int y = 0;

	while (true)
	{
		x = rand() % mapWidth;
		y = rand() % mapHeight;
		if (m_map->getType(x, y) == TYPE_EMPTY)
		{
			m_x = x;
			m_y = y;
			m_map->setType(x, y, TYPE_FOOD);
			
			return;
		}
	}
}