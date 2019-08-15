/*
 * filename : snake.cpp
 * author : PenG
 * create time : 2019/07/31
 * version : 19.07.31
 */

#include "snake.h"

//传入的参数为要操作的地图
Snake::Snake(Map *map)
{
	m_map = map;
	init();
}

Snake::~Snake()
{
	release();
}

//初始化蛇 有数据时会删除数据再初始化
void Snake::init()
{
	//有数据就释放空间
	if (m_wasInit)
	{
		release();
	}

	//蛇初始的位置
	int x, y;
	x = 2;
	y = 1;

	//获取内存空间
	m_tail = new SnakeNode;
	m_tail->last = new SnakeNode;
	m_head = new SnakeNode;

	//把位置信息写入链表中
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

//移动蛇 不能同时与吃食物执行
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

//返回是否吃到食物 需要在获取方向后移动蛇前进行判断
bool Snake::isEatFood() const
{
	int headX = m_head->x;
	int headY = m_head->y;

	directionForward(&headX, &headY, m_direction);

	return m_map->getType(headX, headY) == TYPE_FOOD;
}

//返回是否咬到自己
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

//吃食物 不能同时与移动蛇执行 当事实上不能吃到食物时不会有任何动作
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

	//删除蛇身信息
	SnakeNode *cloneSnakeTail = cloneSnake->m_tail;
	while (cloneSnakeTail->last != NULL)
	{
		cloneSnake->m_tail = cloneSnake->m_tail->last;
		delete cloneSnakeTail;
		cloneSnakeTail = cloneSnake->m_tail;
	}
	delete cloneSnakeTail;

	//复制蛇身信息
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

//转弯 传入参数为要转向的方向
void Snake::turn(char direction)
{
	//传入的方向不能与当前前进方向相反
	if (!isNegativeDirection(direction, m_direction))
	{
		m_direction = direction;
	}
}

//把蛇的数据写到地图中
void Snake::printToMap()
{
	SnakeNode *tmp = m_tail;

	//当是刚初始化完成时
	if (OLD_TAIL_NOT_INIT == m_oldTailX && OLD_TAIL_NOT_INIT == m_oldTailY)
	{
		/* 全部身体都要写一次到地图中 */
		while (tmp != m_head)
		{
			m_map->setType(tmp->x, tmp->y, TYPE_SNAKE_BODY);
			tmp = tmp->last;
		}
		m_map->setType(tmp->x, tmp->y, TYPE_SNAKE_HEAD);

		return;    //此处很关键，没有了的话，下面if之外的代码会被执行
	}
	//当并不是不用擦除时
	else if (!(OLD_TAIL_NOT_ERASE == m_oldTailX && OLD_TAIL_NOT_ERASE == m_oldTailY))
	{
		if (m_map->getType(m_oldTailX, m_oldTailY) != TYPE_FOOD)    //尾结点不是食物时
		{
			// 擦除旧的尾节点
			m_map->setType(m_oldTailX, m_oldTailY, TYPE_EMPTY);
		}
		
	}

	/* 把上一次的蛇头颜色设置为蛇身的颜色，然后写当前的蛇头到地图中 */
	while (tmp->last != m_head)
	{
		tmp = tmp->last;
	}
	m_map->setType(tmp->x, tmp->y, TYPE_SNAKE_BODY);
	m_map->setType(m_head->x, m_head->y, TYPE_SNAKE_HEAD);
}

//返回是否是反方向 turn函数调用
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

//前方方向位置 计算在蛇头direction方向的坐标
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

//释放蛇的数据
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