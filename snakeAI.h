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
	//需要复制地图，蛇和知道食物的位置
	SnakeAI(Map *map, Snake *snake, Food *food);

	//获取下一步的方向
	char getDirection();

private:
	
	//初始化m_pointRecord数组
	void initPointRecordArray();

	//初始化m_pointQueue数组
	void initPointQueueArray();

	//初始化m_way数组
	void initWayArray();

	//克隆地图
	void cloneMap();
	
	//克隆蛇
	void cloneSnake();
	
	//判断蛇头到食物有没有最短路径，有的话会把路径的每一步方向写到m_way中
	bool BFSToFood();
	//BFSToFood调用，用来搜索某一点
	bool BFSSearchPoint(int x, int y, int parentIndex, int *queueTail);
	//BFSToFood调用，用来写路径的每一步方向
	void writeWay(int queueTail);
	//两个writeWay调用，用来判断方向
	char judgeDireciton(int lastX, int lastY, int nextX, int nextY);

	//一步一步移动克隆蛇到食物前
	void moveCloneSnake();

	//判断蛇头此前位置是否能到达蛇尾，能到达时返回真，不能到达时返回假
	bool isWayToTail();
	//isWayToTail调用，用来搜索某一点
	bool isWayToTailSearchPoint(int x, int y, int parentIndex, int *queueTail);

	//寻找蛇头到食物的最长路径，找到时返回真，找不到时返回假
	bool DFSToFood();
	//DFSToFood调用，DFSToFood的重载
	bool DFSToFood(int x, int y);

	//寻找蛇头到蛇尾的最短路径，找到时返回真，找不到时返回假
	bool BFSToTail();
	//BFSToTail调用，用来搜索某一点
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

	char *m_way;    //路径数组
	int m_waySteps;    //路径步数
	bool *m_pointRecord;    //点是否已经遍历过
	int (*m_pointQueue)[3];    //点序列数组

	const int m_mapWidth;
	const int m_mapHeight;
	const int m_mapSize;
};

#endif