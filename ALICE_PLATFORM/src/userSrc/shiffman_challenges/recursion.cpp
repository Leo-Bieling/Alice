//#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zData.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zIO.h>


using namespace zSpace;
using namespace std;

void drawRecursiveCircle(zVector _position, int _n, float _radius)
{
	drawCircle(_position, _n, _radius);
	if (_radius > 0.5)
	{
		drawRecursiveCircle((_position + zVector((_radius * 0.5), 0, 0)), _n, _radius * 0.5);
		drawRecursiveCircle((_position - zVector((_radius * 0.5), 0, 0)), _n, _radius * 0.5);
		drawRecursiveCircle((_position - zVector(0, (_radius * 0.5), 0)), _n, _radius * 0.5);
	}
}

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	
}

void update(int value)
{

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);
	drawRecursiveCircle(zVector(0, 0, 0), 32, 100);

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