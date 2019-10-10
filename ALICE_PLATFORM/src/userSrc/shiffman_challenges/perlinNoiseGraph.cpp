//#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <userSrc/shiffman_challenges/perlinNoise.h>

#include <headers/include/zCore.h>
#include <headers/include/zData.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zIO.h>


using namespace zSpace;
using namespace perlin;
using namespace std;

double start = 0;
double increments = 0.1;
double speed = 0.01;
double y;

vector<zVector> points;

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	S = *new SliderGroup();
	S.addSlider(&increments, "increments");
	S.addSlider(&speed, "speed");

}

void update(int value)
{
	double xOffset = start;

	points.clear();

	for (float x = -20; x < 20; x += 0.5)
	{
		y = noise(xOffset) * 20;
		points.push_back(zVector(x, y, 0.0));
		xOffset += increments;
	}

	start += speed;
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	glColor3d(1, 0, 0.55);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points.size(); i++)
		glVertex3d(points[i].x, points[i].y, points[i].z);
	glEnd();

}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{

}

void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

#endif _MAIN_