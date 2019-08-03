/*
* filename : food.h
* author : PenG
* create time : 2019/07/31
* version : 19.07.31
*/

#pragma once

#ifndef _FOOD_H_
#define _FOOD_H_

class Food
{
public:
	Food(int x, int y)
		:m_x(x), m_y(y) {}

	int getX() { return m_x; }

	int getY() { return m_y; }

private:
	int m_x;
	int m_y;
};

#endif // !_FOOD_H_

