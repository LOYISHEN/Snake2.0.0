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

const char DIRECTION_UP = 1;    //方向 上
const char DIRECTION_RIGHT = 2;    //方向 右
const char DIRECTION_DOWN = 3;    //方向 下
const char DIRECTION_LEFT = 4;    //方向 左

const char FIRST_DIRECTION = DIRECTION_RIGHT;    //初始方向

const int OLD_TAIL_NOT_INIT = -2;    //还没初始化 用于printToMap函数中
const int OLD_TAIL_NOT_ERASE = -1;    //不用擦除 用于printToMap函数中

#include <iostream>
using namespace std;
#include <Windows.h>
#pragma warning( disable : 4996) 

//蛇节点
typedef struct snakeNode
{
	int x;
	int y;
	struct snakeNode *last;
}SnakeNode;

class Snake
{
public:

	//传入的参数为要操作的地图
	Snake(Map *map);

	~Snake();

	//初始化蛇 有数据时会删除数据再初始化
	void init();

	//返回是否已经初始化
	bool wasInit() const { return m_wasInit; }

	//移动蛇 不能同时与吃食物执行
	void move();

	//返回是否吃到食物 需要在获取方向后移动蛇前进行判断
	bool isEatFood() const;

	//返回是否撞到墙
	bool isHitWall() const { return m_map->getType(m_head->x, m_head->y) == TYPE_WALL; }

	//返回是否咬到自己
	bool isBiteSelf() const;

	//吃食物 不能同时与移动蛇执行 当事实上不能吃到食物时不会有任何动作
	void eatFood();

	//转弯 传入参数为要转向的方向
	void turn(char direction);

	Snake *clone(Map *map) const;

	//获取蛇长度
	int getLength() const { return m_length; }

	int getHeadX() const { return m_head->x; }

	int getHeadY() const { return m_head->y; }

	int getTailX() const { return m_tail->x; }

	int getTailY() const { return m_tail->y; }

	char getDirection() const { return m_direction; }

private:

	//返回是否是反方向 turn函数调用
	bool isNegativeDirection(char targetDirection, char currentDirection) const;
	
	//前方方向位置 计算在蛇头direction方向的坐标
	void directionForward(int *x, int *y, char direction) const;

	//把蛇的数据写到地图中
	void printToMap();

	//释放蛇的数据
	void release();

	bool m_wasInit;    //记录是否已经初始化
	int m_length;    //蛇长度
	char m_direction;    //转向方向

	SnakeNode *m_head;    //蛇头数据指针
	SnakeNode *m_tail;    //蛇尾数据指针

	/*
	 * 旧的尾的x和y坐标
	 * 记录移动前的尾的x和y坐标，用于printToMap函数中
	 * OLD_TAIL_NOT_INIT表示还没初始化，OLD_TAIL_NOT_ERASE表示不需要擦除尾节点，非负数表示擦除尾结点
	 */
	int m_oldTailX; int m_oldTailY;

	Map *m_map;    //需要操作的地图的指针
};

#endif
