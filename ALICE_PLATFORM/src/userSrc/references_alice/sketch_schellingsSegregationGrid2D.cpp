#define _MAIN_

#ifdef _MAIN_
#include "main.h"
#include "ALICE_ROBOT_DLL.h"
#include <array>
#include <memory>
#include <time.h>
#include <experimental/generator>
#include "SchellingsSegregationCell2D.h"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace std::experimental;
using namespace ROBOTICS;

#define XRES 200
#define YRES 200
#define ZRES 200

cell basegrid[XRES*YRES*ZRES];


void setup()
{
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < XRES; i++)
	{
		for (int j = 0; j < YRES; j++)
		{
			int id = i * YRES + j;

			cell& c = basegrid[id];
			c.sizeX = 1;
			c.sizeY = 1;
			c.p = vec(i*c.sizeX, j*c.sizeY, 0);
			c.state = ofRandom(0, 1);
		}
	}
}


void update(int value)
{

}

void draw()
{
	backGround(0.25);

	for (int i = (XRES*-1); i < XRES; i++)
	{
		for (int j = 0; j < YRES; j++)
		{
			int id = i * YRES + j;
			cell& c = basegrid[id];
			c.draw();
			//c.state = ofRandom(0, 1);
		}
	}
}

void keyPress(unsigned char k, int xm, int ym)
{

}

void mousePress(int b, int state, int x, int y)
{
	
}

void mouseMotion(int x, int y)
{

}
#endif
