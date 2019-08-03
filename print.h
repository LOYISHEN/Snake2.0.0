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
	Printer(Map *map);    //���캯�� ����Ϊ��ǰ�����ĵ�ͼ

	void printMap();    //��ӡ��ͼ ֻ���ӡ�б仯�ĵ�

private:
	void init();    //��ʼ���������
	void setColor(int color);    //������ɫ
	void setPos(int x, int y);    //���ù��λ��
	void printPix(int x, int y, int color);    //���һ������ɫֵ�����أ������ַ���

	int typeToColor(char type);    //ת����ͼ�ĵ������Ϊ��ɫֵ
	
	Map *m_map;    //��ǰ�����ĵ�ͼ
	Map m_oldMap;    //�ɵĵ�ͼ
};


#endif // !_PRINT_H_