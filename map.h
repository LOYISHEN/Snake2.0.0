/*
* filename : map.h
* author : PenG
* create time : 2019/07/31
* version : 19.07.31
*/

#pragma once

#ifndef _MAP_H_
#define _MAP_H_

//地图的点的类型
const int TYPE_ERROR = -1;    //错误，用于返回错误
const int TYPE_EMPTY = 0;    //空 位置为空
const int TYPE_SNAKE_HEAD = 1;    //蛇头
const int TYPE_SNAKE_BODY = 2;    //蛇身
const int TYPE_FOOD = 3;    //食物
const int TYPE_WALL = 4;    //墙

const int MIN_WIDTH = 6;    //最小地图宽度
const int MIN_HEIGHT = 6;    //最小地图高度
const int MIN_SIZE = MIN_HEIGHT * MIN_WIDTH;    //最小地图大小

#include <iostream>

class Map
{
public:
	Map(int width, int height);    //初始化地图为指定宽度和高度的地图
	Map():m_map(NULL), m_mapHeight(0), m_mapWidth(0), m_mapSize(0){}

	~Map();

	Map(const Map &map);

	Map &operator=(const Map &map);
	
	/* 
	 * 初始化地图为指定宽度和高度的地图
	 * 当已有地图时，会删除该地图，然后新建一个地图
	 * 新建的地图高度不会小于MIN_HEIGHT，宽度不会小于MIN_WIDTH，大小不会小于MIN_SIZE。
	 * 当传入参数不符合上一规则时，会自动调整为此规则的最小数值
	 */
	void init(int width, int height);

	void release();    //释放地图的空间

	/*
	 * 设置地图特定位置的点的类型，类型查看前文的“地图的点的类型”处
	 * 当参数无误时返回真，否则返回假
	 */
	bool setType(int x, int y, char type);

	char getType(int x, int y);    //获取地图特定位置的点的类型 当位置超出地图范围时返回TYPE_ERROR

	int getSize() { return m_mapSize; }
	int getWidth() { return m_mapWidth; }
	int getHeight() { return m_mapHeight; }

private:

	bool typeIsWrong(char type);    //当传入的类型不存在时返回真，存在则返回假
	bool isBetweenHeight(int value) { return value >= 0 && value < m_mapHeight; }    //判断数值是否在地图高度范围内
	bool isBetweenWidth(int value) { return value >= 0 && value < m_mapWidth; }    //判断数值是否在地图宽度范围内

	char *m_map;    //地图 用于存储地图的内存地址
	int m_mapWidth;    //地图宽度
	int m_mapHeight;    //地图高度
	int m_mapSize;    //地图大小
};

#endif // !_MAP_H_
