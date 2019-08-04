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

const int OLD_TAIL_NOT_INIT = -2;    //��û��ʼ�� ����printToMap������
const int OLD_TAIL_NOT_ERASE = -1;    //���ò��� ����printToMap������

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

	bool isHitWall() { return m_map->getType(m_head->x, m_head->y) == TYPE_WALL; }

	bool isBiteSelf();

	void eatFood(int x, int y);

	void turn(char direction);

	int getLength() { return m_length; }

private:

	bool isNegativeDirection(char targetDirection, char currentDirection);

	void directionForward(int *x, int *y, char direction);

	void printToMap();

	void release();

	bool m_wasInit;
	int m_length;
	char m_direction;

	SnakeNode *m_head;
	SnakeNode *m_tail;

	/*
	 * �ɵ�β��x��y����
	 * ��¼�ƶ�ǰ��β��x��y���꣬����printToMap������
	 * OLD_TAIL_NOT_INIT��ʾ��û��ʼ����OLD_TAIL_NOT_ERASE��ʾ����Ҫ����β�ڵ㣬�Ǹ�����ʾ����β���
	 */
	int m_oldTailX; int m_oldTailY;

	Map *m_map;
};

#endif
