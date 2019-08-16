/*
* filename : snake.h
* author : PenG
* create time : 2019/07/31
* version : 19.07.31
*/

#pragma once

#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "map.h"

const char DIRECTION_UP = 1;    //���� ��
const char DIRECTION_RIGHT = 2;    //���� ��
const char DIRECTION_DOWN = 3;    //���� ��
const char DIRECTION_LEFT = 4;    //���� ��

const char FIRST_DIRECTION = DIRECTION_RIGHT;    //��ʼ����

const int OLD_TAIL_NOT_INIT = -2;    //��û��ʼ�� ����printToMap������
const int OLD_TAIL_NOT_ERASE = -1;    //���ò��� ����printToMap������

#include <iostream>
using namespace std;
#include <Windows.h>
#pragma warning( disable : 4996) 

//�߽ڵ�
typedef struct snakeNode
{
	int x;
	int y;
	struct snakeNode *last;
}SnakeNode;

class Snake
{
public:

	//����Ĳ���ΪҪ�����ĵ�ͼ
	Snake(Map *map);

	~Snake();

	//��ʼ���� ������ʱ��ɾ�������ٳ�ʼ��
	void init();

	//�����Ƿ��Ѿ���ʼ��
	bool wasInit() const { return m_wasInit; }

	//�ƶ��� ����ͬʱ���ʳ��ִ��
	void move();

	//�����Ƿ�Ե�ʳ�� ��Ҫ�ڻ�ȡ������ƶ���ǰ�����ж�
	bool isEatFood() const;

	//�����Ƿ�ײ��ǽ
	bool isHitWall() const { return m_map->getType(m_head->x, m_head->y) == TYPE_WALL; }

	//�����Ƿ�ҧ���Լ�
	bool isBiteSelf() const;

	//��ʳ�� ����ͬʱ���ƶ���ִ�� ����ʵ�ϲ��ܳԵ�ʳ��ʱ�������κζ���
	void eatFood();

	//ת�� �������ΪҪת��ķ���
	void turn(char direction);

	Snake *clone(Map *map) const;

	//��ȡ�߳���
	int getLength() const { return m_length; }

	int getHeadX() const { return m_head->x; }

	int getHeadY() const { return m_head->y; }

	int getTailX() const { return m_tail->x; }

	int getTailY() const { return m_tail->y; }

	char getDirection() const { return m_direction; }

private:

	//�����Ƿ��Ƿ����� turn��������
	bool isNegativeDirection(char targetDirection, char currentDirection) const;
	
	//ǰ������λ�� ��������ͷdirection���������
	void directionForward(int *x, int *y, char direction) const;

	//���ߵ�����д����ͼ��
	void printToMap();

	//�ͷ��ߵ�����
	void release();

	bool m_wasInit;    //��¼�Ƿ��Ѿ���ʼ��
	int m_length;    //�߳���
	char m_direction;    //ת����

	SnakeNode *m_head;    //��ͷ����ָ��
	SnakeNode *m_tail;    //��β����ָ��

	/*
	 * �ɵ�β��x��y����
	 * ��¼�ƶ�ǰ��β��x��y���꣬����printToMap������
	 * OLD_TAIL_NOT_INIT��ʾ��û��ʼ����OLD_TAIL_NOT_ERASE��ʾ����Ҫ����β�ڵ㣬�Ǹ�����ʾ����β���
	 */
	int m_oldTailX; int m_oldTailY;

	Map *m_map;    //��Ҫ�����ĵ�ͼ��ָ��
};

#endif
