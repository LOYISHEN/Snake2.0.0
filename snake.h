/*
* filename : snake.h
* author : PenG
* create time : 2019/07/31
* version : 19.07.31
*/

#pragma once

#include <iostream>

#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "map.h"

const char DIRECTION_UP = 1;
const char DIRECTION_RIGHT = 2;
const char DIRECTION_DOWN = 3;
const char DIRECTION_LEFT = 4;

const char FIRST_DIRECTION = DIRECTION_RIGHT;

typedef struct snakeNode
{
	int x;
	int y;
	struct snakeNode *last;
}SnakeNode;

class Snake
{
public:

	Snake(Map *map);

	~Snake();

	void init();

	bool wasInit() { return m_wasInit; }

	void move();

	bool isEatFood(int x, int y);

	void eatFood(int x, int y);

	void turn(char direction);

	void print();

	int getLength() { return m_length; }

private:

	bool isNegativeDirection(char targetDirection, char currentDirection);

	void directionForward(int *x, int *y, char direction);

	void release();

	bool m_wasInit;
	int m_length;
	SnakeNode *m_head;
	SnakeNode *m_tail;
	char m_direction;

	Map *m_map;
};

#endif
