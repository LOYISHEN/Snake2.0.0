/*
* filename : print.h
* author : PenG
* create time : 2019/07/31
* version : 19.07.31
*/

#pragma once

#ifndef _PRINT_H_
#define _PRINT_H_

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>

#include "map.h"

//const int COLOR_ERROR = -1;
const int COLOR_EMPTY = 0x0000;
const int COLOR_SNAKE_BODY = 0x0010;
const int COLOR_SNAKE_HEAD = 0x0040;
const int COLOR_FOOD = 0x0060;
const int COLOR_WALL = 0x00f0;

class Printer
{
public:
	Printer(Map *map, int left = 0, int top = 0);    //构造函数 参数为当前操作的地图

	void printPix(int x, int y, int color, char *ch = NULL);    //输出一个带颜色值的像素（两个字符）

	void printMap();    //打印地图 只会打印有变化的点

	int typeToColor(char type);    //转换地图的点的类型为颜色值

private:
	void init();    //初始化输出设置
	void setColor(int color);    //设置颜色
	void setPos(int x, int y);    //设置光标位置
	
	Map *m_map;    //当前操作的地图
	Map m_oldMap;    //旧的地图

	int m_left;
	int m_top;
};


#endif // !_PRINT_H_
