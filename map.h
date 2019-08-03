/*
* filename : map.h
* author : PenG
* create time : 2019/07/31
* version : 19.07.31
*/

#pragma once

#ifndef _MAP_H_
#define _MAP_H_

//��ͼ�ĵ������
const int TYPE_ERROR = -1;    //�������ڷ��ش���
const int TYPE_EMPTY = 0;    //�� λ��Ϊ��
const int TYPE_SNAKE_HEAD = 1;    //��ͷ
const int TYPE_SNAKE_BODY = 2;    //����
const int TYPE_FOOD = 3;    //ʳ��
const int TYPE_WALL = 4;    //ǽ

const int MIN_WIDTH = 6;    //��С��ͼ���
const int MIN_HEIGHT = 6;    //��С��ͼ�߶�
const int MIN_SIZE = MIN_HEIGHT * MIN_WIDTH;    //��С��ͼ��С

#include <iostream>

class Map
{
public:
	Map(int width, int height);    //��ʼ����ͼΪָ����Ⱥ͸߶ȵĵ�ͼ
	Map():m_map(NULL), m_mapHeight(0), m_mapWidth(0), m_mapSize(0){}

	~Map();

	Map(const Map &map);

	Map &operator=(const Map &map);
	
	/* 
	 * ��ʼ����ͼΪָ����Ⱥ͸߶ȵĵ�ͼ
	 * �����е�ͼʱ����ɾ���õ�ͼ��Ȼ���½�һ����ͼ
	 * �½��ĵ�ͼ�߶Ȳ���С��MIN_HEIGHT����Ȳ���С��MIN_WIDTH����С����С��MIN_SIZE��
	 * �����������������һ����ʱ�����Զ�����Ϊ�˹������С��ֵ
	 */
	void init(int width, int height);

	void release();    //�ͷŵ�ͼ�Ŀռ�

	/*
	 * ���õ�ͼ�ض�λ�õĵ�����ͣ����Ͳ鿴ǰ�ĵġ���ͼ�ĵ�����͡���
	 * ����������ʱ�����棬���򷵻ؼ�
	 */
	bool setType(int x, int y, char type);

	char getType(int x, int y);    //��ȡ��ͼ�ض�λ�õĵ������ ��λ�ó�����ͼ��Χʱ����TYPE_ERROR

	int getSize() { return m_mapSize; }
	int getWidth() { return m_mapWidth; }
	int getHeight() { return m_mapHeight; }

private:

	bool typeIsWrong(char type);    //����������Ͳ�����ʱ�����棬�����򷵻ؼ�
	bool isBetweenHeight(int value) { return value >= 0 && value < m_mapHeight; }    //�ж���ֵ�Ƿ��ڵ�ͼ�߶ȷ�Χ��
	bool isBetweenWidth(int value) { return value >= 0 && value < m_mapWidth; }    //�ж���ֵ�Ƿ��ڵ�ͼ��ȷ�Χ��

	char *m_map;    //��ͼ ���ڴ洢��ͼ���ڴ��ַ
	int m_mapWidth;    //��ͼ���
	int m_mapHeight;    //��ͼ�߶�
	int m_mapSize;    //��ͼ��С
};

#endif // !_MAP_H_
