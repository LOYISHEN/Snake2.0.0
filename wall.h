/*
* filename : wall.h
* author : PenG
* create time : 2019/08/12
* version : 19.08.12
*/

#pragma once

#ifndef _WALL_H_
#define _WALL_H_

#include "map.h"

class Wall
{
public:
	Wall(Map *map)
	{
		m_map = map;
	}

	void printToMap()
	{
		int mapHeight = m_map->getHeight();
		int mapWidth = m_map->getWidth();

		for (int x = 0; x < mapWidth; x++)
		{
			m_map->setType(x, 0, TYPE_WALL);
			m_map->setType(x, mapHeight-1, TYPE_WALL);
		}

		for (int y = 0; y < mapHeight; y++)
		{
			m_map->setType(0, y, TYPE_WALL);
			m_map->setType(mapWidth-1, y, TYPE_WALL);
		}
	}

private:
	Map *m_map;
};

#endif