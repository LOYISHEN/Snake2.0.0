/*
* filename : snakeAI.h
* author : PenG
* create time : 2019/08/13
* version : 19.08.13
*/

#pragma once

#ifndef _SNAKEAI_H_
#define _SNAKEAI_H_

#include "map.h"
#include "snake.h"
#include "food.h"
#include "print.h"
#include <Windows.h>

class SnakeAI
{
public:
	//��Ҫ���Ƶ�ͼ���ߺ�֪��ʳ���λ��
	SnakeAI(Map *map, Snake *snake, Food *food);

	//��ȡ��һ���ķ���
	char getDirection();

private:
	
	//��ʼ��m_pointRecord����
	void initPointRecordArray();

	//��ʼ��m_pointQueue����
	void initPointQueueArray();

	//��ʼ��m_way����
	void initWayArray();

	//��¡��ͼ
	void cloneMap();
	
	//��¡��
	void cloneSnake();
	
	//�ж���ͷ��ʳ����û�����·�����еĻ����·����ÿһ������д��m_way��
	bool BFSToFood();
	//BFSToFood���ã���������ĳһ��
	bool BFSSearchPoint(int x, int y, int parentIndex, int *queueTail);
	//BFSToFood���ã�����д·����ÿһ������
	void writeWay(int queueTail);
	//����writeWay���ã������жϷ���
	char judgeDireciton(int lastX, int lastY, int nextX, int nextY);

	//һ��һ���ƶ���¡�ߵ�ʳ��ǰ
	void moveCloneSnake();

	//�ж���ͷ��ǰλ���Ƿ��ܵ�����β���ܵ���ʱ�����棬���ܵ���ʱ���ؼ�
	bool isWayToTail();
	//isWayToTail���ã���������ĳһ��
	bool isWayToTailSearchPoint(int x, int y, int parentIndex, int *queueTail);

	//Ѱ����ͷ��ʳ����·�����ҵ�ʱ�����棬�Ҳ���ʱ���ؼ�
	bool DFSToFood();
	//DFSToFood���ã�DFSToFood������
	bool DFSToFood(int x, int y);

	//Ѱ����ͷ����β�����·�����ҵ�ʱ�����棬�Ҳ���ʱ���ؼ�
	bool BFSToTail();
	//BFSToTail���ã���������ĳһ��
	bool BFSSearchPointToTail(int x, int y, int parentIndex, int *queueTail);

	bool findLongestWayToTail();
	bool DFSToTail(int x, int y, int steps);
	bool test(int x, int y, int steps);

	int *m_tailBFSSteps;
	void tailBFS();
	void initTailBFSStepsArray();
	void tailBFSSearchPoint(int x, int y, int parentIndex, int *queueTail, int steps);
	char distinguishDirection();


	const Map *m_map;
	Map *m_cloneMap;
	const Snake *m_snake;
	Snake *m_cloneSnake;
	const Food *m_food;

	Printer *m_printer;

	char m_direction;

	char *m_way;    //·������
	int m_waySteps;    //·������
	bool *m_pointRecord;    //���Ƿ��Ѿ�������
	int (*m_pointQueue)[3];    //����������

	const int m_mapWidth;
	const int m_mapHeight;
	const int m_mapSize;
};

#endif