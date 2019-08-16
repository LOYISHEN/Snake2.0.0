/*
* filename : snakeAI.cpp
* author : PenG
* create time : 2019/08/13
* version : 19.08.13
*/

#include "snakeAI.h"

SnakeAI::SnakeAI(Map *map, Snake *snake, Food *food)
	:m_mapWidth(m_map->getWidth()), m_mapHeight(m_map->getHeight()), m_mapSize(m_map->getSize()),
	m_map(map), m_snake(snake), m_food(food), m_waySteps(0)
{
	m_cloneMap = new Map(map->getWidth(), map->getHeight());

	m_cloneSnake = new Snake(m_cloneMap);

	m_pointRecord = new bool[m_mapSize];
	initPointRecordArray();

	m_pointQueue = new int[m_mapSize][3];
	initPointQueueArray();

	m_way = new char[m_mapSize];
	initWayArray();

	m_printer = new Printer(m_cloneMap, 21);

	m_tailBFSSteps = new int[m_mapSize];
}

void SnakeAI::initPointRecordArray()
{
	for (int i = 0; i < m_map->getSize(); i++)
	{
		m_pointRecord[i] = false;
	}
}

void SnakeAI::initPointQueueArray()
{
	for (int i = 0; i < m_map->getSize(); i++)
	{
		m_pointQueue[i][0] = -1;
		m_pointQueue[i][1] = -1;
		m_pointQueue[i][2] = -1;
	}
}

void SnakeAI::initWayArray()
{
	for (int i = 0; i < m_mapSize; i++)
	{
		m_way[i] = -1;
	}
}

/*
 *
 * ˼·��
 * 1. �����ҵ�����ʳ������·�����ҵ�ʱ������һ������������5��
 * 2. �ƶ���¡�ߺ��ж��Ƿ��ܹ�������β���������ҵ��˷��򣬷��������һ��
 * 3. �ָ��ƶ���¡��ǰ��������ж��Ƿ��ܹ��ҵ�������β�����·�����������ҵ��˷��򣬷��������һ��
 * 4. �ָ��ƶ���¡��ǰ��������ж��Ƿ��ܹ��ҵ�����ʳ����·�����������ҵ��˷�����������е��飩�������Ҳ�������
 * 5. �ж��ܷ��ҵ�������β�����·�����������ҵ��˷��򣬷����Ҳ�������
 *
 */

//��ȡ��һ�����ƶ�����
char SnakeAI::getDirection()
{
	//���ȿ�¡��ͼ����
	cloneMap();
	cloneSnake();

	//�������·����ʳ�ﴦʱ
	if (BFSToFood())
	{
		char message[128] = { 0 };
		sprintf(message, "min way to food\n");
		OutputDebugString(message);

		//�ƶ���¡��ȥ��ʳ��
		moveCloneSnake();
		//������ʳ��֮����·���ߵ�β��ʱ
		if (isWayToTail())
		{
			//���ط���
			m_direction = m_way[0];
			return m_direction;
		}

		sprintf(message, "min way to food but no way to tail\n");
		OutputDebugString(message);

		cloneMap();
		cloneSnake();
		//������ʳ��֮��û��·���ߵ�β����Ѱ��û���ƶ���¡��ǰ��ͷ������β���·���ɹ�ʱ
		if (BFSToTail())
		{
			if (m_snake->getLength() + 20 >= m_mapSize - 2 * m_mapHeight - 2 * m_mapWidth + 4)
			{
				findLongestWayToTail();
				return m_direction;
			}
			else
			{
				cloneMap();
				cloneSnake();
				tailBFS();
				return distinguishDirection();
			}
			//m_direction = m_way[0];
			//return m_direction;
		}

		
		sprintf(message, "min way to food but find way to tail\n");
		OutputDebugString(message);

		cloneMap();
		cloneSnake();
		//��û�е�����β���·����Ѱ��û���ƶ���¡��ǰ��ͷ����ʳ����·���ɹ�ʱ
		if (DFSToFood())
		{
			return m_direction;
		}
		
		//cout << "�޽⣡" << endl;
		throw "�޽�";
		return -1;
	}

	char message[128] = { 0 };
	sprintf(message, "no min way to food\n");
	OutputDebugString(message);

	cloneMap();
	cloneSnake();
	//��û��·����ʳ��ʱ��Ѱ�ҵ�����β���·��
	if(BFSToTail())
	{
		if (m_snake->getLength() + 10 >= m_mapSize)
		{
			findLongestWayToTail();
			return m_direction;
		}
		else
		{
			cloneMap();
			cloneSnake();
			tailBFS();
			return distinguishDirection();
		}
		//m_direction = m_way[0];
		//return m_direction;
	}
	
	//cout << "�޽⣡" << endl;
	return m_snake->getDirection();
	throw "�޽�";
	return -1;
}

void SnakeAI::cloneMap()
{
	for (int y = 0; y < m_map->getHeight(); y++)
	{
		for (int x = 0; x < m_map->getWidth(); x++)
		{
			m_cloneMap->setType(x, y, m_map->getType(x, y));
		}
	}
}

void SnakeAI::cloneSnake()
{
	delete m_cloneSnake;
	m_cloneSnake = m_snake->clone(m_cloneMap);
}

bool SnakeAI::BFSToFood()
{
	/* ������� */
	initPointQueueArray();
	initPointRecordArray();
	initWayArray();
	m_waySteps = 0;

	/* ����ͷ��ʼ���� */
	m_pointQueue[0][0] = -1;
	m_pointQueue[0][1] = m_cloneSnake->getHeadX();
	m_pointQueue[0][2] = m_cloneSnake->getHeadY();

	int queueTail = 1;
	int x = -1;
	int y = -1;

	/* ��ȱ��� */
	for (int parentIndex = 0; parentIndex < m_mapSize - 1; parentIndex++)
	{
		//���Ѿ�û�п��Ա����ĵ�ʱ����false
		if (m_pointQueue[parentIndex][0] == -1 && parentIndex != 0)
		{
			return false;
		}

		x = m_pointQueue[parentIndex][1];
		y = m_pointQueue[parentIndex][2];

		//����
		if (BFSSearchPoint(x, y - 1, parentIndex, &queueTail)    //����
			|| BFSSearchPoint(x + 1, y, parentIndex, &queueTail)    //����
			|| BFSSearchPoint(x, y + 1, parentIndex, &queueTail)    //����
			|| BFSSearchPoint(x - 1, y, parentIndex, &queueTail))    //����
		{
			return true;
		}

	}    // for end
	return false;
}

bool SnakeAI::BFSSearchPoint(int x, int y, int parentIndex, int *queueTail)
{
	char type = m_cloneMap->getType(x, y);
	int BFSRecordIndex = y * m_mapHeight + x;    //BFSRecord���������

	if ((type == TYPE_EMPTY || type == TYPE_FOOD) && !m_pointRecord[BFSRecordIndex])
	{
		m_pointQueue[(*queueTail)][0] = parentIndex;
		m_pointQueue[(*queueTail)][1] = x;
		m_pointQueue[(*queueTail)][2] = y;

		m_pointRecord[BFSRecordIndex] = true;    //��ʶ�Ѿ�ʶ����õ�

		(*queueTail)++;

		if (type == TYPE_FOOD)
		{
			//�ڵ��ñ�����ǰ�Ѿ���ʼ����m_way������
			writeWay((*queueTail) - 1);    //��·��д����
			return true;
		}
	}
	return false;
}

void SnakeAI::writeWay(int queueTail)
{
	int sum = 0;
	while (queueTail != 0)
	{
		m_way[sum] = judgeDireciton(m_pointQueue[m_pointQueue[queueTail][0]][1], m_pointQueue[m_pointQueue[queueTail][0]][2]
									, m_pointQueue[queueTail][1], m_pointQueue[queueTail][2]);
		queueTail = m_pointQueue[queueTail][0];

		sum++;
	}
	m_waySteps = sum;

	/* ��ת */
	char direction;
	for (int down = 0, up = m_waySteps - 1; down < up; down++, up--)
	{
		direction = m_way[up];
		m_way[up] = m_way[down];
		m_way[down] = direction;
	}
}

char SnakeAI::judgeDireciton(int lastX, int lastY, int nextX, int nextY)
{
	if (nextX == lastX)
	{
		if (nextY > lastY)
		{
			return DIRECTION_DOWN;
		}
		else
		{
			return DIRECTION_UP;
		}
	}
	else if (nextY == lastY)
	{
		if (nextX > lastX)
		{
			return DIRECTION_RIGHT;
		}
		else
		{
			return DIRECTION_LEFT;
		}
	}
}

void SnakeAI::moveCloneSnake()
{
	//�����һ���϶��ǳ�ʳ��
	int steps = 0;
	for (; steps < m_waySteps - 1; steps++)
	{
		m_cloneSnake->turn(m_way[steps]);
		m_cloneSnake->move();
		m_printer->printMap();
	}
	m_cloneSnake->turn(m_way[steps]);
	if (m_cloneSnake->isEatFood())
	{
		m_cloneSnake->eatFood();
	}
	else
	{
		m_cloneSnake->move();
	}
	m_printer->printMap();
}

bool SnakeAI::isWayToTail()
{
	/* ������� */
	initPointQueueArray();
	initPointRecordArray();

	/* ����ͷ��ʼ���� */
	m_pointQueue[0][0] = -1;
	m_pointQueue[0][1] = m_cloneSnake->getHeadX();
	m_pointQueue[0][2] = m_cloneSnake->getHeadY();

	int queueTail = 1;
	int x = -1;
	int y = -1;

	/* ��ȱ��� */
	for (int parentIndex = 0; parentIndex < m_mapSize - 1; parentIndex++)
	{
		//���Ѿ�û�п��Ա����ĵ�ʱ����false
		if (m_pointQueue[parentIndex][0] == -1 && parentIndex != 0)
		{
			return false;
		}

		x = m_pointQueue[parentIndex][1];
		y = m_pointQueue[parentIndex][2];

		//����
		if (isWayToTailSearchPoint(x, y - 1, parentIndex, &queueTail)    //����
			|| isWayToTailSearchPoint(x + 1, y, parentIndex, &queueTail)    //����
			|| isWayToTailSearchPoint(x, y + 1, parentIndex, &queueTail)    //����
			|| isWayToTailSearchPoint(x - 1, y, parentIndex, &queueTail))    //����
		{
			return true;
		}
	}    // for end
	return false;
}

bool SnakeAI::isWayToTailSearchPoint(int x, int y, int parentIndex, int *queueTail)
{
	char type = m_cloneMap->getType(x, y);
	int BFSRecordIndex = y * m_mapHeight + x;    //BFSRecord���������
	bool isTailPoint = m_cloneSnake->getTailX() == x && m_cloneSnake->getTailY() == y;

	if ((type == TYPE_EMPTY || (type == TYPE_SNAKE_BODY && isTailPoint))
		&& !m_pointRecord[BFSRecordIndex])
	{
		if (isTailPoint)
		{
			return true;
		}
		m_pointQueue[(*queueTail)][0] = parentIndex;
		m_pointQueue[(*queueTail)][1] = x;
		m_pointQueue[(*queueTail)][2] = y;

		m_pointRecord[BFSRecordIndex] = true;    //��ʶ�Ѿ�ʶ����õ�

		(*queueTail)++;
	}
	return false;
}

bool SnakeAI::DFSToFood()
{
	initPointRecordArray();
	int x = m_cloneSnake->getHeadX();
	int y = m_cloneSnake->getHeadY();

	if (DFSToFood(x, y - 1))
	{
		m_direction = DIRECTION_UP;
		return true;
	}

	if (DFSToFood(x + 1, y))
	{
		m_direction = DIRECTION_RIGHT;
		return true;
	}

	if (DFSToFood(x, y + 1))
	{
		m_direction = DIRECTION_DOWN;
		return true;
	}

	if (DFSToFood(x - 1, y))
	{
		m_direction = DIRECTION_LEFT;
		return true;
	}

	return false;
}

bool SnakeAI::DFSToFood(int x, int y)
{
	int recordIndex = y * m_mapHeight + x;
	if (m_cloneMap->getType(x, y) == TYPE_FOOD)
	{
		return true;
	}
	else if (m_cloneMap->getType(x, y) == TYPE_EMPTY && !m_pointRecord[recordIndex])
	{
		m_pointRecord[recordIndex] = true;
		if (DFSToFood(x, y - 1)
			|| DFSToFood(x + 1, y)
			|| DFSToFood(x, y + 1)
			|| DFSToFood(x - 1, y))
		{
			m_pointRecord[recordIndex] = false;
			return true;
		}
		m_pointRecord[recordIndex] = false;
	}
	return false;
}

bool SnakeAI::BFSToTail()
{
	initPointQueueArray();
	initWayArray();
	initPointRecordArray();
	m_waySteps = 0;

	/* ����ͷ��ʼ���� */
	m_pointQueue[0][0] = -1;
	m_pointQueue[0][1] = m_cloneSnake->getHeadX();
	m_pointQueue[0][2] = m_cloneSnake->getHeadY();

	int queueTail = 1;
	int x = -1;
	int y = -1;

	/* ��ȱ��� */
	for (int parentIndex = 0; parentIndex < m_mapSize - 1; parentIndex++)
	{
		//���Ѿ�û�п��Ա����ĵ�ʱ����false
		if (m_pointQueue[parentIndex][0] == -1 && parentIndex != 0)
		{
			return false;
		}

		x = m_pointQueue[parentIndex][1];
		y = m_pointQueue[parentIndex][2];

		//����
		if (BFSSearchPointToTail(x, y - 1, parentIndex, &queueTail)    //����
			|| BFSSearchPointToTail(x + 1, y, parentIndex, &queueTail)    //����
			|| BFSSearchPointToTail(x, y + 1, parentIndex, &queueTail)    //����
			|| BFSSearchPointToTail(x - 1, y, parentIndex, &queueTail))    //����
		{
			return true;
		}

	}    // for end
	return false;
}

bool SnakeAI::BFSSearchPointToTail(int x, int y, int parentIndex, int *queueTail)
{
	char type = m_cloneMap->getType(x, y);
	int BFSRecordIndex = BFSRecordIndex = y * m_mapHeight + x;    //BFSRecord���������
	bool isPointToTail = m_cloneSnake->getTailX() == x && m_cloneSnake->getTailY() == y;

	if ((type == TYPE_EMPTY || (type == TYPE_SNAKE_BODY && isPointToTail)) 
		 && !m_pointRecord[BFSRecordIndex])
	{
		m_pointQueue[(*queueTail)][0] = parentIndex;
		m_pointQueue[(*queueTail)][1] = x;
		m_pointQueue[(*queueTail)][2] = y;

		m_pointRecord[BFSRecordIndex] = true;    //��ʶ�Ѿ�ʶ����õ�

		(*queueTail)++;

		if (isPointToTail)
		{
			//�ڵ��ñ�����ǰ�Ѿ���ʼ����m_way������
			writeWay((*queueTail) - 1);    //��·��д����
			return true;
		}
	}
	return false;
}

bool SnakeAI::findLongestWayToTail()
{
	initPointQueueArray();
	initPointRecordArray();
	m_waySteps = 0;

	int x = m_cloneSnake->getHeadX();
	int y = m_cloneSnake->getHeadY();

	m_pointQueue[0][0] = -1;
	m_pointQueue[0][1] = x;
	m_pointQueue[0][2] = y;

	bool isFoundLongestWay = DFSToTail(x, y - 1, 0);
	isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y + 1, 0);
	isFoundLongestWay = isFoundLongestWay || DFSToTail(x + 1, y, 0);
	isFoundLongestWay = isFoundLongestWay || DFSToTail(x - 1, y, 0);

	if (isFoundLongestWay)
	{
		return true;
	}

	return false;
}

bool SnakeAI::DFSToTail(int x, int y, int steps)
{
	if (x <= 0 || y <= 0 || x >= m_mapWidth - 1 || y >= m_mapHeight - 1)
	{
		return false;
	}
	int recordIndex = y * m_map->getHeight() + x;
	if (x == m_cloneSnake->getTailX() && y == m_cloneSnake->getTailY())
	{
		//Ϊ�˼��ټ���������������������ʱ����ֹͣ����
		if (steps > m_cloneSnake->getLength() || m_waySteps < steps)
		{
			m_waySteps = steps;
			//ֱ���жϵ�һ�����������
			m_direction = judgeDireciton(m_pointQueue[0][1], m_pointQueue[0][2], m_pointQueue[1][1], m_pointQueue[1][2]);
			m_printer->printPix(x, y, COLOR_FOOD);
			m_printer->printMap();
			m_printer->printPix(x, y, COLOR_EMPTY);
			m_printer->printMap();
			return true;
		}
	}
	else if (m_cloneMap->getType(x, y) == TYPE_EMPTY && !m_pointRecord[recordIndex])
	{
		m_pointRecord[recordIndex] = true;
		m_pointQueue[steps + 1][0] = steps;
		m_pointQueue[steps + 1][1] = x;
		m_pointQueue[steps + 1][2] = y;
		m_printer->printPix(x, y, COLOR_FOOD);
		m_printer->printMap();
		//Sleep(1);

		bool isFoundLongestWay = test(x, y, steps);
		//bool isFoundLongestWay = DFSToTail(x, y - 1, steps + 1);
		//isFoundLongestWay = isFoundLongestWay || DFSToTail(x + 1, y, steps + 1);
		//isFoundLongestWay = isFoundLongestWay || DFSToTail(x - 1, y, steps + 1);
		//isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y + 1, steps + 1);

		m_printer->printPix(x, y, COLOR_EMPTY);
		m_printer->printMap();
		m_pointRecord[recordIndex] = false;
		m_pointQueue[steps + 1][0] = -1;
		m_pointQueue[steps + 1][1] = -1;
		m_pointQueue[steps + 1][2] = -1;

		if (isFoundLongestWay)
		{
			return true;
		}
	}
	return false;
}

bool SnakeAI::test(int x, int y, int steps)
{
	bool isFoundLongestWay = false;

	switch (m_direction)
	{
	case DIRECTION_DOWN:
		if (m_food->getX() < x)
		{
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y - 1, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x + 1, y, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x - 1, y, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y + 1, steps + 1);
		}
		else
		{
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y - 1, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x - 1, y, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x + 1, y, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y + 1, steps + 1);
		}
		break;
		
	case DIRECTION_LEFT:
		if (m_food->getY() < y)
		{
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x + 1, y, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y + 1, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y - 1, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x - 1, y, steps + 1);
		}
		else
		{
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x + 1, y, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y - 1, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y + 1, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x - 1, y, steps + 1);

		}
		break;

	case DIRECTION_UP:
		if (m_food->getX() < x)
		{
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y + 1, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x + 1, y, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x - 1, y, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y - 1, steps + 1);
		}
		else
		{
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y + 1, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x - 1, y, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x + 1, y, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y - 1, steps + 1);
		}
		
		break;

	case DIRECTION_RIGHT:
		if (m_food->getY() < y)
		{
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x - 1, y, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y + 1, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y - 1, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x + 1, y, steps + 1);
		}
		else
		{
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x - 1, y, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y - 1, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y + 1, steps + 1);
			isFoundLongestWay = isFoundLongestWay || DFSToTail(x + 1, y, steps + 1);
		}
		break;
		
	default:
		isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y - 1, steps + 1);
		isFoundLongestWay = isFoundLongestWay || DFSToTail(x + 1, y, steps + 1);
		isFoundLongestWay = isFoundLongestWay || DFSToTail(x - 1, y, steps + 1);
		isFoundLongestWay = isFoundLongestWay || DFSToTail(x, y + 1, steps + 1);
		break;
	}
	return isFoundLongestWay;
}

void SnakeAI::initTailBFSStepsArray()
{
	for (int i = 0; i < m_mapSize; i++)
	{
		m_tailBFSSteps[i] = -1;
	}
}

void SnakeAI::tailBFS()
{
	/* ������� */
	initPointQueueArray();
	initTailBFSStepsArray();
	initWayArray();
	m_waySteps = 0;

	/* ����β��ʼ���� */
	m_pointQueue[0][0] = -1;
	m_pointQueue[0][1] = m_cloneSnake->getTailX();
	m_pointQueue[0][2] = m_cloneSnake->getTailY();

	int queueTail = 1;
	int x = -1;
	int y = -1;

	int steps = 1;
	int lastPointParent = -1;

	/* ��ȱ��� */
	for (int parentIndex = 0; parentIndex < m_mapSize - 1; parentIndex++)
	{
		//���Ѿ�û�п��Ա����ĵ�ʱ����
		if (m_pointQueue[parentIndex][0] == -1 && parentIndex != 0)
		{
			return;
		}

		if (lastPointParent != m_pointQueue[parentIndex][0])
		{
			lastPointParent = m_pointQueue[parentIndex][0];
			steps++;
		}

		x = m_pointQueue[parentIndex][1];
		y = m_pointQueue[parentIndex][2];

		tailBFSSearchPoint(x, y - 1, parentIndex, &queueTail, steps);    //����
		tailBFSSearchPoint(x + 1, y, parentIndex, &queueTail, steps);    //����
		tailBFSSearchPoint(x, y + 1, parentIndex, &queueTail, steps);    //����
		tailBFSSearchPoint(x - 1, y, parentIndex, &queueTail, steps);    //����

	}    // for end
	return;
}

void SnakeAI::tailBFSSearchPoint(int x, int y, int parentIndex, int *queueTail, int steps)
{
	char type = m_cloneMap->getType(x, y);
	int tailBFSStepsIndex = y * m_mapHeight + x;

	if (type == TYPE_EMPTY && m_tailBFSSteps[tailBFSStepsIndex] == -1)
	{
		m_pointQueue[(*queueTail)][0] = steps;
		m_pointQueue[(*queueTail)][1] = x;
		m_pointQueue[(*queueTail)][2] = y;

		m_tailBFSSteps[tailBFSStepsIndex] = steps;    //��ʶ����

		(*queueTail)++;
	}
	return;
}

char SnakeAI::distinguishDirection()
{
	int directionSteps[4][2] = { -1, -1, -1, -1, -1, -1, -1, -1};
	int headX = m_cloneSnake->getHeadX();
	int headY = m_cloneSnake->getHeadY();
	directionSteps[0][0] = 0;
	directionSteps[0][1] = m_tailBFSSteps[(headY - 1) * m_mapHeight + headX];
	directionSteps[1][0] = 1;
	directionSteps[1][1] = m_tailBFSSteps[headY * m_mapHeight + headX + 1];
	directionSteps[2][0] = 2;
	directionSteps[2][1] = m_tailBFSSteps[(headY + 1) * m_mapHeight + headX];
	directionSteps[3][0] = 3;
	directionSteps[3][1] = m_tailBFSSteps[headY * m_mapHeight + headX - 1];
	
	/* ���� */
	for (int j = 0; j < 4; j++)
	{
		for (int i = j + 1; i < 4; i++)
		{
			if (directionSteps[i][1] > directionSteps[j][1])
			{
				int steps = directionSteps[i][1];
				int index = directionSteps[i][0];
				directionSteps[i][1] = directionSteps[j][1];
				directionSteps[i][0] = directionSteps[j][0];
				directionSteps[j][1] = steps;
				directionSteps[j][0] = index;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (directionSteps[i][1] == -1)
		{
			continue;
		}
		cloneMap();
		cloneSnake();
		m_printer->printMap();
		m_waySteps = 1;
		m_way[0] = directionSteps[i][0] + 1;   //direction��1234��������0123

		moveCloneSnake();

		if (isWayToTail())
		{
			m_direction = m_way[0];
			return m_direction;
		}
	}
	if (BFSToTail())
	{
		m_direction = m_way[0];
		return m_direction;
	}
}