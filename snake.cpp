/*
 * filename : snake.cpp
 * author : PenG
 * create time : 2019/07/31
 * version : 19.07.31
 */

#include "snake.h"

Snake::Snake(Map *map)
{
	init();
	m_map = map;
}

Snake::~Snake()
{
	release();
}

void Snake::init()
{
	if (m_wasInit)
	{
		release();
	}

	//�߳�ʼ��λ��
	int x, y;
	x = 2;
	y = 1;

	//��ȡ�ڴ�ռ�
	m_tail = new SnakeNode;
	m_tail->last = new SnakeNode;
	m_head = new SnakeNode;

	//��λ����Ϣд��������
	m_tail->x = x - 1;
	m_tail->y = y;
	m_tail->last->x = x;
	m_tail->last->y = y;
	m_tail->last->last = m_head;
	m_head->x = x + 1;
	m_head->y = y;
	m_head->last = NULL;
	
	m_wasInit = true;
	m_length = 3;

	m_direction = FIRST_DIRECTION;

	m_oldTailX = -1;
	m_oldTailY = -1;
	printToMap();
}

void Snake::move()
{
	int x = m_head->x;
	int y = m_head->y;
	m_oldTailX = m_tail->x;
	m_oldTailY = m_tail->y;

	directionForward(&x, &y, m_direction);

	SnakeNode *temp = m_tail;
	while (temp != m_head)
	{
		temp->x = temp->last->x;
		temp->y = temp->last->y;
		temp = temp->last;
	}
	m_head->x = x;
	m_head->y = y;

	printToMap();
}

bool Snake::isEatFood(int x, int y)
{
	int headX = m_head->x;
	int headY = m_head->y;

	directionForward(&headX, &headY, m_direction);

	return headX == x && headY == y;
}

bool Snake::isBiteSelf()
{
	SnakeNode *tmp = m_tail;
	while (tmp->last != m_head)
	{
		if (tmp->x == m_head->x && tmp->y == m_head->y)
		{
			return true;
		}
		tmp = tmp->last;
	}
	return false;
}

void Snake::eatFood(int x, int y)
{
	if (isEatFood(x, y))
	{
		SnakeNode *node = new SnakeNode;
		node->last = NULL;
		node->x = x;
		node->y = y;
		m_head->last = node;
		m_head = node;
	}
	m_oldTailX = OLD_TAIL_NOT_ERASE;
	m_oldTailY = OLD_TAIL_NOT_ERASE;

	printToMap();
}

void Snake::turn(char direction)
{
	if (!isNegativeDirection(direction, m_direction))
	{
		m_direction = direction;
	}
}


//���ߵ�����д����ͼ��
void Snake::printToMap()
{
	SnakeNode *tmp = m_tail;
	if (OLD_TAIL_NOT_INIT == m_oldTailX && OLD_TAIL_NOT_INIT == m_oldTailY)
	{
		/* ȫ�����嶼Ҫдһ�ε���ͼ�� */
		while (tmp != m_head)
		{
			m_map->setType(tmp->x, tmp->y, TYPE_SNAKE_BODY);
			tmp = tmp->last;
		}
		m_map->setType(tmp->x, tmp->y, TYPE_SNAKE_HEAD);

		return;    //�˴��ܹؼ���û���˵Ļ�������if֮��Ĵ���ᱻִ��
	}
	else if (!(OLD_TAIL_NOT_ERASE == m_oldTailX && OLD_TAIL_NOT_ERASE == m_oldTailY))    //�������ǲ��ò���ʱ
	{
		/* �����ɵ�β�ڵ㣬����һ�ε���ͷ��ɫ����Ϊ�������ɫ��Ȼ��д��ǰ����ͷ����ͼ�� */
		m_map->setType(m_oldTailX, m_oldTailY, TYPE_EMPTY);
	}

	/* ����һ�ε���ͷ��ɫ����Ϊ�������ɫ��Ȼ��д��ǰ����ͷ����ͼ�� */
	while (tmp->last != m_head)
	{
		tmp = tmp->last;
	}
	m_map->setType(tmp->x, tmp->y, TYPE_SNAKE_BODY);
	m_map->setType(m_head->x, m_head->y, TYPE_SNAKE_HEAD);
}

bool Snake::isNegativeDirection(char targetDirection, char currentDirection)
{
	switch (targetDirection)
	{
	case DIRECTION_UP:
		return DIRECTION_DOWN == currentDirection;
		break;

	case DIRECTION_RIGHT:
		return DIRECTION_LEFT == currentDirection;
		break;

	case DIRECTION_DOWN:
		return DIRECTION_UP == currentDirection;
		break;

	case DIRECTION_LEFT:
		return DIRECTION_RIGHT == currentDirection;
		break;

	default:
		return false;
		break;
	}
	return false;
}

void Snake::directionForward(int *x, int *y, char direction)
{
	switch (direction)
	{
	case DIRECTION_UP:
		(*y)--;
		break;

	case DIRECTION_RIGHT:
		(*x)++;
		break;

	case DIRECTION_DOWN:
		(*y)++;
		break;

	case DIRECTION_LEFT:
		(*x)--;
		break;
	default:
		break;
	}
}

void Snake::release()
{
	SnakeNode *tmp = this->m_tail;

	while (tmp != NULL)
	{
		this->m_tail = this->m_tail->last;
		delete tmp;
		tmp = this->m_tail;
	}

	this->m_tail = NULL;
	this->m_head = NULL;
	this->m_wasInit = false;
}