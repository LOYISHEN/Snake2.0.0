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

	m_BFSRecord = new bool[m_mapSize];
	initBFSRecordArray();

	m_DFSRecord = new bool[m_mapSize];
	initDFSRecordArray();

	m_DFSWay = new int[m_mapSize][3];
	initDFSWayArray();

	m_BFSQueue = new int[m_mapSize][3];
	initBFSQueueArray();

	m_way = new char[m_mapSize];
	initWayArray();
}

void SnakeAI::initDFSRecordArray()
{
	for (int i = 0; i < m_map->getSize(); i++)
	{
		m_DFSRecord[i] = false;
	}
}

void SnakeAI::initBFSRecordArray()
{
	for (int i = 0; i < m_map->getSize(); i++)
	{
		m_BFSRecord[i] = false;
	}
}

void SnakeAI::initBFSQueueArray()
{
	for (int i = 0; i < m_map->getSize(); i++)
	{
		m_BFSQueue[i][0] = -1;
		m_BFSQueue[i][1] = -1;
		m_BFSQueue[i][2] = -1;
	}
}

void SnakeAI::initDFSWayArray()
{
	for (int i = 0; i < m_map->getSize(); i++)
	{
		m_DFSWay[i][0] = -1;
		m_DFSWay[i][1] = -1;
		m_DFSWay[i][2] = -1;
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
 * 3. �ָ��ƶ���¡��ǰ��������ж��Ƿ��ܹ��ҵ�������β���·�����������ҵ��˷��򣬷��������һ��
 * 4. �ָ��ƶ���¡��ǰ��������ж��Ƿ��ܹ��ҵ�����ʳ����·�����������ҵ��˷�����������е��飩�������Ҳ�������
 * 5. �ж��ܷ��ҵ�������β���·�����������ҵ��˷��򣬷����Ҳ�������
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
		if (DFSToTail())
		{
			return m_direction;
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
	if(DFSToTail())
	{
		return m_direction;
	}
	
	//cout << "�޽⣡" << endl;
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
	initBFSQueueArray();
	initBFSRecordArray();
	initWayArray();
	m_waySteps = 0;

	/* ����ͷ��ʼ���� */
	m_BFSQueue[0][0] = -1;
	m_BFSQueue[0][1] = m_cloneSnake->getHeadX();
	m_BFSQueue[0][2] = m_cloneSnake->getHeadY();

	int queueTail = 1;
	int x = -1;
	int y = -1;

	/* ��ȱ��� */
	for (int parentIndex = 0; parentIndex < m_mapSize - 1; parentIndex++)
	{
		//���Ѿ�û�п��Ա����ĵ�ʱ����false
		if (m_BFSQueue[parentIndex][0] == -1 && parentIndex != 0)
		{
			return false;
		}

		x = m_BFSQueue[parentIndex][1];
		y = m_BFSQueue[parentIndex][2];

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
	char type = TYPE_EMPTY;
	int BFSRecordIndex = BFSRecordIndex = y * m_mapHeight + x;    //BFSRecord���������

	type = m_cloneMap->getType(x, y);
	if ((type == TYPE_EMPTY || type == TYPE_FOOD) && !m_BFSRecord[BFSRecordIndex])
	{
		m_BFSQueue[(*queueTail)][0] = parentIndex;
		m_BFSQueue[(*queueTail)][1] = x;
		m_BFSQueue[(*queueTail)][2] = y;

		m_BFSRecord[BFSRecordIndex] = true;    //��ʶ�Ѿ�ʶ����õ�

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
		m_way[sum] = judgeDireciton(m_BFSQueue[m_BFSQueue[queueTail][0]][1], m_BFSQueue[m_BFSQueue[queueTail][0]][2]
									, m_BFSQueue[queueTail][1], m_BFSQueue[queueTail][2]);
		queueTail = m_BFSQueue[queueTail][0];

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
	}
	m_cloneSnake->turn(m_way[steps]);
	m_cloneSnake->eatFood();
}

bool SnakeAI::isWayToTail()
{
	/* ������� */
	initBFSQueueArray();
	initBFSRecordArray();

	/* ����ͷ��ʼ���� */
	m_BFSQueue[0][0] = -1;
	m_BFSQueue[0][1] = m_cloneSnake->getHeadX();
	m_BFSQueue[0][2] = m_cloneSnake->getHeadY();

	int queueTail = 1;
	int x = -1;
	int y = -1;

	/* ��ȱ��� */
	for (int parentIndex = 0; parentIndex < m_mapSize - 1; parentIndex++)
	{
		//���Ѿ�û�п��Ա����ĵ�ʱ����false
		if (m_BFSQueue[parentIndex][0] == -1 && parentIndex != 0)
		{
			return false;
		}

		x = m_BFSQueue[parentIndex][1];
		y = m_BFSQueue[parentIndex][2];

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
		&& !m_BFSRecord[BFSRecordIndex])
	{
		if (isTailPoint)
		{
			return true;
		}
		m_BFSQueue[(*queueTail)][0] = parentIndex;
		m_BFSQueue[(*queueTail)][1] = x;
		m_BFSQueue[(*queueTail)][2] = y;

		m_BFSRecord[BFSRecordIndex] = true;    //��ʶ�Ѿ�ʶ����õ�

		(*queueTail)++;
	}
	return false;
}

bool SnakeAI::DFSToFood()
{
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
	if (m_cloneMap->getType(x, y) == TYPE_FOOD)
	{
		return true;
	}
	else if (m_cloneMap->getType(x, y) == TYPE_EMPTY)
	{
		if (DFSToFood(x, y - 1)
			|| DFSToFood(x + 1, y)
			|| DFSToFood(x, y + 1)
			|| DFSToFood(x - 1, y))
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool SnakeAI::DFSToTail()
{
	initDFSRecordArray();
	initWayArray();
	initDFSWayArray();
	m_waySteps = 0;

	int x = m_cloneSnake->getHeadX();
	int y = m_cloneSnake->getHeadY();

	int (*currentDFSWays)[3];
	currentDFSWays = new int[m_map->getSize()][3];

	if (!DFSToTail(x, y - 1, 0, currentDFSWays))
	{
		if (!DFSToTail(x + 1, y, 0, currentDFSWays))
		{
			if (!DFSToTail(x, y + 1, 0, currentDFSWays))
			{
				if (!DFSToTail(x - 1, y, 0, currentDFSWays))
				{
					return false;
				}
			}
		}
	}

	writeWay(m_DFSWay);
	if ((m_direction = m_way[0]) != -1)
	{
		return true;
	}

	return false;
}

void SnakeAI::writeWay(int (*DFSWays)[3])
{
	for (int i = 0; i < m_map->getSize(); i++)
	{
		if (-1 == DFSWays[i + 1][0])
		{
			break;
		}
		m_way[i] = judgeDireciton(m_DFSWay[i][1], m_DFSWay[i][2], m_DFSWay[i + 1][1], m_DFSWay[i + 1][2]);
	}
}

bool SnakeAI::DFSToTail(int x, int y, int steps, int (*currentDFSWays)[3])
{
	int DFSRecordIndex = y * m_map->getHeight() + x;
	if (m_waySteps > m_snake->getLength())
	{
		char message[128] = { 0 };

		sprintf(message, "m_waySteps is : %d and return\n", m_waySteps);
		OutputDebugString(message);
		return true;
	}
	if (x == m_cloneSnake->getTailX() && y == m_cloneSnake->getTailY())
	{
		if (steps > m_waySteps)
		{
			replaceDFSWayByCurrentDFSWays(currentDFSWays);
			m_waySteps = steps;

			char message[128] = { 0 };
			sprintf(message, "m_waySteps is : %d\n", m_waySteps);
			OutputDebugString(message);
		}
	}
	else if (m_cloneMap->getType(x, y) == TYPE_EMPTY && !m_DFSRecord[DFSRecordIndex])
	{
		m_DFSRecord[DFSRecordIndex] = true;
		currentDFSWays[steps][0] = steps;
		currentDFSWays[steps][1] = x;
		currentDFSWays[steps][2] = y;

		if (DFSToTail(x, y - 1, steps + 1, currentDFSWays)) return true;
		if (DFSToTail(x + 1, y, steps + 1, currentDFSWays)) return true;
		if (DFSToTail(x, y + 1, steps + 1, currentDFSWays)) return true;
		if (DFSToTail(x - 1, y, steps + 1, currentDFSWays)) return true;

		m_DFSRecord[DFSRecordIndex] = false;
		currentDFSWays[steps][0] = -1;
		currentDFSWays[steps][1] = -1;
		currentDFSWays[steps][2] = -1;
	}

	return false;
}

void SnakeAI::replaceDFSWayByCurrentDFSWays(int(*currentDFSWays)[3])
{
	initDFSWayArray();
	for (int i = 0; i < m_map->getSize(); i++)
	{
		if (-1 == currentDFSWays[i][0])
		{
			char message[128] = { 0 };
			sprintf(message, "break\n");
			OutputDebugString(message);
			break;
		}
		m_DFSWay[i][0] = currentDFSWays[i][0];
		m_DFSWay[i][1] = currentDFSWays[i][1];
		m_DFSWay[i][2] = currentDFSWays[i][2];
	}
}