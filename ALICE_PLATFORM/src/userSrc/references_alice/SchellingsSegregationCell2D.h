#ifndef CELL
#define CELL
#include "main.h"

class cell
{
public:
	vec p;
	float sizeX;
	float sizeY;
	float state;

	cell()
	{
		sizeX = 1;
		sizeY = 1;
	}

	cell(vec _p, int _sizeX, int _sizeY, int _state)
	{
		p = _p;
		sizeX = _sizeX;
		sizeY = _sizeY;
		state = _state;
	}

	void draw()
	{
		if (state > 0.2 && state <= 0.4)
		{
			glColor3f(1, 0, 1);
		}
		else if (state > 0.4 && state <= 0.6)
		{
			glColor3f(0.2, 0.2, 0.2);
		}
		else
		{
			glColor3f(1, 1, 1);
		}

		drawRectangle(p, p + vec(sizeX, sizeY, 0));
	}
	
};

#endif 
#pragma once
