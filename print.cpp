/*
* filename : print.cpp
* author : PenG
* create time : 2019/07/31
* version : 19.07.31
*/

#include "print.h"

HANDLE hOut;    //用于输出的句柄
COORD pos = { 0,0 };    //输出的位置
CONSOLE_CURSOR_INFO cci;    //定义结构体

//构造函数 参数为当前操作的地图
Printer::Printer(Map *map, int left, int top)
{
	init();
	m_map = map;
	m_oldMap = *map;
	m_top = top;
	m_left = left;
}

//打印地图 只会打印有变化的点
void Printer::printMap()
{
	//找出与旧的地图不同的地方打印对应点
	for (int y = 0; y < m_oldMap.getHeight(); y++)
	{
		for (int x = 0; x < m_oldMap.getWidth(); x++)
		{
			if (m_oldMap.getType(x, y) != m_map->getType(x, y)
				&& m_oldMap.getType(x, y) != TYPE_ERROR && m_map->getType(x, y) != TYPE_ERROR)
			{
				printPix(x + m_left, y + m_top, typeToColor(m_map->getType(x, y)));
			}
		}
	}
	m_oldMap = *m_map;
}

//初始化输出设置
void Printer::init()
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleCursorInfo(hOut, &cci);    //获取光标信息
	cci.dwSize = 1;    //设置光标大小
	cci.bVisible = 0; //设置光标不可见 FALSE
	SetConsoleCursorInfo(hOut, &cci);    //设置(应用)光标信息
	pos.X = 0;
	pos.Y = 0;
	SetConsoleCursorPosition(hOut, pos);    //设置光标坐标
}

//设置颜色
void Printer::setColor(int color)
{
	SetConsoleTextAttribute(hOut, color);
}

//设置光标位置
void Printer::setPos(int x, int y)
{
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hOut, pos);
}

//输出一个带颜色值的像素（两个字符）
void Printer::printPix(int x, int y, int color, char *ch)
{
	if (NULL == ch)
	{
		setPos(2 * x, y);
		setColor(color);
		printf("  ");
	}
	else
	{
		setPos(2 * x, y);
		setColor(color);
		printf("%s", ch);
	}
}

//转换地图的点的类型为颜色值
int Printer::typeToColor(char type)
{
	switch (type)
	{
	case TYPE_EMPTY:
		return COLOR_EMPTY;
		break;

	case TYPE_SNAKE_BODY:
		return COLOR_SNAKE_BODY;
		break;

	case TYPE_SNAKE_HEAD:
		return COLOR_SNAKE_HEAD;
		break;

	case TYPE_FOOD:
		return COLOR_FOOD;
		break;

	case TYPE_WALL:
		return COLOR_WALL;
		break;

	default:
		return COLOR_EMPTY;
		break;
	}
}