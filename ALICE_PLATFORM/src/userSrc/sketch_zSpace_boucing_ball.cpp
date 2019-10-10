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

zVector location;
zVector velocity;
zVector gravity;

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	location = zVector(0, 0, 40);
	velocity = zVector(0.1, 0.1, -0.2);
	gravity = zVector(0, 0, -0.02);
}

void update(int value)
{
	// add velocity to the location
	location += velocity;
	// add gravity to the velocity
	velocity += gravity;

	// bounce off the boundary
	if (location.x > 20 || location.x < -20) velocity.x *= -1;
	if (location.y > 20 || location.y < -20) velocity.y *= -1;
	if (location.z < 0) velocity.z *= -0.93;

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	drawPoint(location, zColor(1, 0, 0, 1), 6.0);

	//cout << "x= " << location.x << " y= " << location.y << " z= " << location.z << endl;
}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 's') location = zVector(0, 0, 20);
}

void mousePress(int b, int state, int x, int y)
{
	
}

void mouseMotion(int x, int y)
{
	
}

#endif _MAIN_