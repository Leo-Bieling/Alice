#ifndef CELL
#define CELL
#include "main.h"

class cell
{
public:
	vec p;
	int sizeX;
	int sizeY;

	cell()
	{
		sizeX = 1;
		sizeY = 1;
	}

	cell(vec _p, int _sizeX, int _sizeY)
	{
		p = _p;
		sizeX = _sizeX;
		sizeY = _sizeY;
	}

	void draw()
	{
		glColor3f(0.2, 0.2, 0.2);
		wireFrameOn();
		drawRectangle(p, p + vec(sizeX, sizeY,0));
	}

	void drawNeighbours()
	{
		glColor3f(1, 0.2, 0.2);
		wireFrameOff();
		drawRectangle(p, p + vec(sizeX, sizeY, 0));
		
	}

};

#endif 
#pragma once
