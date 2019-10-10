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

double start = 0.0;
double increments = 0.02;
double speed = 0.0;
double z;
bool color = false;
vector<zVector> points;

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	S = *new SliderGroup();
	S.addSlider(&increments, "increments");
	S.addSlider(&speed, "speed");
	B.addButton(&color, "color");
}

void update(int value)
{
	double xOffset = 0;
	double yOffset = 0;

	points.clear();

	for (float x = -20; x < 20; x += 0.1)
	{
		xOffset = start;
		for (float y = -20; y < 20; y += 0.1)
		{
			z = noise(xOffset, yOffset) * 20;
			points.push_back(zVector(x, y, z));
			xOffset += increments;
		}
		yOffset += increments;
	}
	start += speed;
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);


	for (int i = 0; i < points.size(); i++)
	{
		zColor col = zColor(1.0, 1.0, 1.0, 1.0);
		if (color)
		{
			double value = ofMap(points[i].z, -20, 20, 0, 1);
			col = zColor(1.0, value, value, 1.0);
		}
		drawPoint(points[i], col, 2);
	}

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