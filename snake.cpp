/*
 * filename : snake.cpp
 * author : PenG
 * create time : 2019/07/31
 * version : 19.07.31
 */

#include "snake.h"

//����Ĳ���ΪҪ�����ĵ�ͼ
Snake::Snake(Map *map)
{
	m_map = map;
	init();
}

Snake::~Snake()
{
	release();
}

//��ʼ���� ������ʱ��ɾ�������ٳ�ʼ��
void Snake::init()
{
	//�����ݾ��ͷſռ�
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

	m_oldTailX = OLD_TAIL_NOT_INIT;
	m_oldTailY = OLD_TAIL_NOT_INIT;
	printToMap();
}

//�ƶ��� ����ͬʱ���ʳ��ִ��
void Snake::move()
{
	m_oldTailX = m_tail->x;
	m_oldTailY = m_tail->y;

	int x = m_head->x;
	int y = m_head->y;
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

//�����Ƿ�Ե�ʳ�� ��Ҫ�ڻ�ȡ������ƶ���ǰ�����ж�
bool Snake::isEatFood() const
{
	int headX = m_head->x;
	int headY = m_head->y;

	directionForward(&headX, &headY, m_direction);

	return m_map->getType(headX, headY) == TYPE_FOOD;
}

//�����Ƿ�ҧ���Լ�
bool Snake::isBiteSelf() const
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

//��ʳ�� ����ͬʱ���ƶ���ִ�� ����ʵ�ϲ��ܳԵ�ʳ��ʱ�������κζ���
void Snake::eatFood()
{
	int x = m_head->x;
	int y = m_head->y;
	directionForward(&x, &y, m_direction);

	if (isEatFood())
	{
		SnakeNode *node = new SnakeNode;
		node->last = NULL;
		node->x = x;
		node->y = y;
		m_head->last = node;
		m_head = node;

		m_length++;

		m_oldTailX = OLD_TAIL_NOT_ERASE;
		m_oldTailY = OLD_TAIL_NOT_ERASE;
		printToMap();
	}
}

Snake *Snake::clone(Map *map) const
{
	Snake *cloneSnake = new Snake(map);

	//ɾ��������Ϣ
	SnakeNode *cloneSnakeTail = cloneSnake->m_tail;
	while (cloneSnakeTail->last != NULL)
	{
		cloneSnake->m_tail = cloneSnake->m_tail->last;
		delete cloneSnakeTail;
		cloneSnakeTail = cloneSnake->m_tail;
	}
	delete cloneSnakeTail;

	//����������Ϣ
	SnakeNode *thisSnakeCurrentNode = this->m_tail;
	cloneSnakeTail = NULL;
	SnakeNode *cloneSnakeLastNode = NULL;
	SnakeNode *cloneSnakeCurrentNode = NULL;
	while (thisSnakeCurrentNode != this->m_head)
	{
		cloneSnakeCurrentNode = new SnakeNode;
		cloneSnakeCurrentNode->x = thisSnakeCurrentNode->x;
		cloneSnakeCurrentNode->y = thisSnakeCurrentNode->y;
		
		if (NULL == cloneSnakeTail)
		{
			cloneSnakeTail = cloneSnakeCurrentNode;
		}
		
		if (NULL != cloneSnakeLastNode)
		{
			cloneSnakeLastNode->last = cloneSnakeCurrentNode;
		}
		
		cloneSnakeLastNode = cloneSnakeCurrentNode;

		thisSnakeCurrentNode = thisSnakeCurrentNode->last;
	}
	cloneSnake->m_head = new SnakeNode;
	cloneSnake->m_head->last = NULL;
	cloneSnake->m_head->x = this->m_head->x;
	cloneSnake->m_head->y = this->m_head->y;
	cloneSnakeCurrentNode->last = cloneSnake->m_head;

	cloneSnake->m_tail = cloneSnakeTail;

	cloneSnake->m_direction = m_direction;
	cloneSnake->m_length = m_length;
	
	return cloneSnake;
}

//ת�� �������ΪҪת��ķ���
void Snake::turn(char direction)
{
	//����ķ������뵱ǰǰ�������෴
	if (!isNegativeDirection(direction, m_direction))
	{
		m_direction = direction;
	}
}

//���ߵ�����д����ͼ��
void Snake::printToMap()
{
	SnakeNode *tmp = m_tail;

	//���Ǹճ�ʼ�����ʱ
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
	//�������ǲ��ò���ʱ
	else if (!(OLD_TAIL_NOT_ERASE == m_oldTailX && OLD_TAIL_NOT_ERASE == m_oldTailY))
	{
		if (m_map->getType(m_oldTailX, m_oldTailY) != TYPE_FOOD)    //β��㲻��ʳ��ʱ
		{
			// �����ɵ�β�ڵ�
			m_map->setType(m_oldTailX, m_oldTailY, TYPE_EMPTY);
		}
		
	}

	/* ����һ�ε���ͷ��ɫ����Ϊ�������ɫ��Ȼ��д��ǰ����ͷ����ͼ�� */
	while (tmp->last != m_head)
	{
		tmp = tmp->last;
	}
	m_map->setType(tmp->x, tmp->y, TYPE_SNAKE_BODY);
	m_map->setType(m_head->x, m_head->y, TYPE_SNAKE_HEAD);
}

//�����Ƿ��Ƿ����� turn��������
bool Snake::isNegativeDirection(char targetDirection, char currentDirection) const
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

//ǰ������λ�� ��������ͷdirection���������
void Snake::directionForward(int *x, int *y, char direction) const
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

//�ͷ��ߵ�����
void Snake::release()
{
	SnakeNode *tmp = this->m_tail;

	while (tmp->last != NULL)
	{
		this->m_tail = this->m_tail->last;
		delete tmp;
		tmp = this->m_tail;
	}
	delete tmp;

	this->m_tail = NULL;
	this->m_head = NULL;
	this->m_wasInit = false;
}