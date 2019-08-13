/*
* filename : map.cpp
* author : PenG
* create time : 2019/07/31
* version : 19.07.31
*/

#include "map.h"

//��ʼ����ͼΪָ����Ⱥ͸߶ȵĵ�ͼ
Map::Map(int width, int height)
{
	init(width, height);
}

Map::~Map()
{
	release();
}

Map::Map(const Map &map)
{
	//����������ͼ�����ݣ�ɾ������
	if (m_mapSize > 0)
	{
		delete m_map;
	}
	m_map = new char[map.m_mapSize];    //��ȡ�µĿռ�
	//��������
	memcpy(m_map, map.m_map, map.m_mapSize);
	m_mapHeight = map.m_mapHeight;
	m_mapWidth = map.m_mapWidth;
	m_mapSize = map.m_mapSize;
}

Map &Map::operator=(const Map &map)
{
	//����������ͼ�����ݣ�ɾ������
	if (m_mapSize > 0)
	{
		delete m_map;
	}
	m_map = new char[map.m_mapSize];    //��ȡ�µĿռ�
	//��������
	memcpy(m_map, map.m_map, map.m_mapSize);
	m_mapHeight = map.m_mapHeight;
	m_mapWidth = map.m_mapWidth;
	m_mapSize = map.m_mapSize;

	return *this;
}

/*
 * ��ʼ����ͼΪָ����Ⱥ͸߶ȵĵ�ͼ
 * �����е�ͼʱ����ɾ���õ�ͼ��Ȼ���½�һ����ͼ
 * �½��ĵ�ͼ�߶Ȳ���С��MIN_HEIGHT����Ȳ���С��MIN_WIDTH����С����С��MIN_SIZE��
 * �����������������һ����ʱ�����Զ�����Ϊ�˹������С��ֵ
 */
void Map::init(int width, int height)
{
	if (m_mapSize > 0)
	{
		delete m_map;
	}

	width = width < MIN_WIDTH ? MIN_WIDTH : width;
	height = height < MIN_HEIGHT ? MIN_HEIGHT : height;

	m_map = new char[height * width];
	m_mapHeight = height;
	m_mapWidth = width;
	m_mapSize = height * width;

	for (int i = 0; i < m_mapSize; i++)
	{
		m_map[i] = TYPE_EMPTY;
	}
}

//�ͷŵ�ͼ�Ŀռ�
void Map::release()
{
	if (m_mapSize > 0)
	{
		delete m_map;
		m_mapHeight = 0;
		m_mapWidth = 0;
		m_mapSize = 0;
	}
}

/*
 * ���õ�ͼ�ض�λ�õĵ�����ͣ����Ͳ鿴ǰ�ĵġ���ͼ�ĵ�����͡���
 * ����������ʱ�����棬���򷵻ؼ�
 */
bool Map::setType(int x, int y, char type)
{
	if (m_mapSize > 0 && isBetweenWidth(x) && isBetweenHeight(y) && !typeIsWrong(type))
	{
		m_map[y * m_mapHeight + x] = type;
		return true;
	}
	return false;
}

//��ȡ��ͼ�ض�λ�õĵ������ ��λ�ó�����ͼ��Χʱ����TYPE_ERROR
char Map::getType(int x, int y)
{
	if (m_mapSize > 0 && isBetweenWidth(x) && isBetweenHeight(y))
	{
		return m_map[y * m_mapHeight + x];
	}
	return TYPE_ERROR;
}

//����������Ͳ�����ʱ�����棬�����򷵻ؼ�
bool Map::typeIsWrong(char type)
{
	switch (type)
	{
	case TYPE_EMPTY:
	case TYPE_SNAKE_HEAD:
	case TYPE_SNAKE_BODY:
	case TYPE_FOOD:
	case TYPE_WALL:
		return false;
		break;

	default:
		return true;
		break;
	}
}