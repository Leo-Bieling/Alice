#define _MAIN_

#ifdef _MAIN_
#include "main.h"
#include "ALICE_ROBOT_DLL.h"
#include <array>
#include <memory>
#include <time.h>
#include <experimental/generator>
#include "cell2D.h"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace std::experimental;
using namespace ROBOTICS;

#define XRES 20
#define YRES 20
#define ZRES 20

cell basegrid[XRES*YRES];

vector<cell> neighbours;


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
			c.sizeX = 5;
			c.sizeY = 5;
			c.p = vec(i*c.sizeX, j*c.sizeY, 0);
		}
	}


// NOW evaluating one randon cell with index 15 -- NEXT is to evaluate all the cells at the same time


	int tempXpos = floor(basegrid[15].p.x);					// get temporary position along X of the cell you want to evaluate the neighbours
	int tempYpos = floor(basegrid[15].p.y);					// get temporary position along Y of the cell you want to evaluate the neighbours

	for (int u = tempXpos - 1; u <= tempXpos + 1; u++)		// index based search around the temporary position along X
	{
		for (int v = tempYpos - 1; v <= tempYpos + 1; v++)	// index based search around the temporary position along Y
		{
			int id = u * YRES + v;							// retrieve the index of the neighbour cells	
			cell& neighbourCells = basegrid[id];			
			neighbours.push_back(neighbourCells);			// fill the vector for potential future use
		}
	}
}


void update(int value)
{
				
}

void draw()
{
	backGround(1);

	for (int i = 0; i < XRES; i++)
	{
		for (int j = 0; j < YRES; j++)
		{
			int id = i * YRES + j;

			cell& c = basegrid[id];
			c.draw();
		}
	}

	for (int i = 0; i < neighbours.size(); i++) neighbours[i].drawNeighbours();	// visualize the neighbour cells stored in the vector
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
