/*
* filename : map.cpp
* author : PenG
* create time : 2019/07/31
* version : 19.07.31
*/

#include "map.h"

//初始化地图为指定宽度和高度的地图
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
	//如果本对象地图有数据，删除数据
	if (m_mapSize > 0)
	{
		delete m_map;
	}
	m_map = new char[map.m_mapSize];    //获取新的空间
	//复制数据
	memcpy(m_map, map.m_map, map.m_mapSize);
	m_mapHeight = map.m_mapHeight;
	m_mapWidth = map.m_mapWidth;
	m_mapSize = map.m_mapSize;
}

Map &Map::operator=(const Map &map)
{
	//如果本对象地图有数据，删除数据
	if (m_mapSize > 0)
	{
		delete m_map;
	}
	m_map = new char[map.m_mapSize];    //获取新的空间
	//复制数据
	memcpy(m_map, map.m_map, map.m_mapSize);
	m_mapHeight = map.m_mapHeight;
	m_mapWidth = map.m_mapWidth;
	m_mapSize = map.m_mapSize;

	return *this;
}

/*
 * 初始化地图为指定宽度和高度的地图
 * 当已有地图时，会删除该地图，然后新建一个地图
 * 新建的地图高度不会小于MIN_HEIGHT，宽度不会小于MIN_WIDTH，大小不会小于MIN_SIZE。
 * 当传入参数不符合上一规则时，会自动调整为此规则的最小数值
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

//释放地图的空间
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
 * 设置地图特定位置的点的类型，类型查看前文的“地图的点的类型”处
 * 当参数无误时返回真，否则返回假
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

//获取地图特定位置的点的类型 当位置超出地图范围时返回TYPE_ERROR
char Map::getType(int x, int y)
{
	if (m_mapSize > 0 && isBetweenWidth(x) && isBetweenHeight(y))
	{
		return m_map[y * m_mapHeight + x];
	}
	return TYPE_ERROR;
}

//当传入的类型不存在时返回真，存在则返回假
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