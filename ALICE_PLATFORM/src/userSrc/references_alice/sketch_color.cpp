#define _MAIN_

#ifdef _MAIN_

#include "main.h"


void setup()
{

}

void update(int value)
{

}

void draw()
{
	backGround(0.75);

	glPointSize(7); // set point size
	glEnable(GL_POINT_SMOOTH); // enable smooth display of points.

	for (int i = -20; i < 80; i++)
	{
		vec4 clr = getColour(i, -20, 80); // convert a single float number to a color value between the min and max values.
		glColor3f(clr.r, clr.g, clr.b); // use that color, to set the color the next point that will be drawn
			
		drawPoint(vec(i, 0, 0)); // draw a point and at specific vector position.	
	}
}

void keyPress(unsigned char k, int xm, int ym)
{

}

void mousePress(int b, int s, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

#endif
