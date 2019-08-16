/*
* filename : print.cpp
* author : PenG
* create time : 2019/07/31
* version : 19.07.31
*/

#include "print.h"

HANDLE hOut;    //��������ľ��
COORD pos = { 0,0 };    //�����λ��
CONSOLE_CURSOR_INFO cci;    //����ṹ��

//���캯�� ����Ϊ��ǰ�����ĵ�ͼ
Printer::Printer(Map *map, int left, int top)
{
	init();
	m_map = map;
	m_oldMap = *map;
	m_top = top;
	m_left = left;
}

//��ӡ��ͼ ֻ���ӡ�б仯�ĵ�
void Printer::printMap()
{
	//�ҳ���ɵĵ�ͼ��ͬ�ĵط���ӡ��Ӧ��
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

//��ʼ���������
void Printer::init()
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleCursorInfo(hOut, &cci);    //��ȡ�����Ϣ
	cci.dwSize = 1;    //���ù���С
	cci.bVisible = 0; //���ù�겻�ɼ� FALSE
	SetConsoleCursorInfo(hOut, &cci);    //����(Ӧ��)�����Ϣ
	pos.X = 0;
	pos.Y = 0;
	SetConsoleCursorPosition(hOut, pos);    //���ù������
}

//������ɫ
void Printer::setColor(int color)
{
	SetConsoleTextAttribute(hOut, color);
}

//���ù��λ��
void Printer::setPos(int x, int y)
{
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hOut, pos);
}

//���һ������ɫֵ�����أ������ַ���
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

//ת����ͼ�ĵ������Ϊ��ɫֵ
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