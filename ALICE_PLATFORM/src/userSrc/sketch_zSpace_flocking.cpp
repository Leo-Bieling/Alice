//#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zData.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zIO.h>

#include <userSrc/sketch_zSpace_flocking_boid.h>

using namespace zSpace;
using namespace std;

vector<Boid> flock;


////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	for (int i = 0; i < 1; i++) flock.push_back(Boid(zVector(randomNumber(-20, 20), randomNumber(-20, 20), 0)));

	for (Boid b : flock) b.position.x + 20;

}

void update(int value)
{
	for (Boid b : flock) b.update(flock);
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	for (Boid b : flock) b.draw();
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