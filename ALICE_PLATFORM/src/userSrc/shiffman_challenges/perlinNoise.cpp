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

zVector position = zVector(0, 0, 0);
double perlinX = 0.0;
double perlinY = 0.0;
double perlinInput1 = 0;
double perlinInput2 = 1000;

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
}

void update(int value)
{
	perlinInput1 += 0.01;
	perlinInput2 += 0.01;
	perlinX = noise(perlinInput1) * 20;
	perlinY = noise(perlinInput2) * 20;
	position = zVector(perlinX, perlinY, 0);
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	drawPoint(position, zColor(1, 0, (1 / 255 * 100), 1), 10.0);
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