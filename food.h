/*
* filename : food.h
* author : PenG
* create time : 2019/07/31
* version : 19.07.31
*/

#pragma once

#ifndef _FOOD_H_
#define _FOOD_H_

#include "map.h"

#include <time.h>

class Food
{
public:
	Food(Map *map): m_map(map), m_x(-1), m_y(-1){}

	void set();

	int getX() const { return m_x; }

	int getY() const { return m_y; }

private:
	int m_x;
	int m_y;
	Map *m_map;
};

#endif // !_FOOD_H_

