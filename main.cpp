/*
* filename : main.cpp
* author : PenG
* create time : 2019/08/12
* version : 19.08.12
*/

#define _CRT_SECURE_NO_WARNINGS

#include "game.h"
#include <iostream>

using namespace std;

int main()
{
	game::Game game(10, 10, 1);

	game.start();

	system("pause");
	return 0;
}