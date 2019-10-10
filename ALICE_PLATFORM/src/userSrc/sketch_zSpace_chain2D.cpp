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

///////////////////////////////////////////////////////////////CLASS
class Spring2D
{
	
public:
	Spring2D();
	~Spring2D();
	Spring2D(float xpos, float ypos, float m, float g);
	void update(float targetX, float targetY);
	void display(float nx, float ny);

	float vx, vy; // the x- and y-velocity
	float x, y; // the x- and y-coordinates
	float gravity;
	float mass;
	float dotSize = 10;
	float stiffness = 1.0;
	float damping = 0.1;

private:

};

Spring2D::Spring2D()
{
}

Spring2D::~Spring2D()
{
}

Spring2D::Spring2D(float xpos, float ypos, float m, float g)
{
	x = xpos;
	y = ypos;
	mass = m;
	gravity = g;
}

void Spring2D::update(float targetX, float targetY)
{
	float forceX = (targetX - x) * stiffness;
	float ax = forceX / mass;
	vx = damping * (vx + ax);
	x += vx;
	float forceY = (targetY - y) * stiffness;
	forceY += gravity;
	float ay = forceY / mass;
	vy = damping * (vy + ay);
	y += vy;
}

void Spring2D::display(float nx, float ny)
{
	drawPoint(zVector(nx, ny, 0), zColor(1, 0, 0, 1), dotSize);
	drawLine(zVector(x, y, 0), zVector(nx, ny, 0), zColor(0, 0, 0, 1), 2.0);
}

///////////////////////////////////////////////////////////////CLASS END

Spring2D s1, s2, s3, s4;

float gravity = -9.8;
float mass = 1.0;
zVector position = zVector(0.0, 0.0, 0.0);

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	s1 = Spring2D(0.0, 0.0, mass, gravity);
	s2 = Spring2D(0.0, 0.0, mass, gravity);
	s3 = Spring2D(0.0, 0.0, mass, gravity);
	s4 = Spring2D(0.0, 0.0, mass, gravity);

}

void update(int value)
{
	s1.update(position.x, position.y);
	s2.update(s1.x, s1.y);
	s3.update(s2.x, s2.y);
	s4.update(s3.x, s3.y);

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	s1.display(position.x, position.y);
	s2.display(s1.x, s1.y);
	s3.display(s2.x, s2.y);
	s4.display(s3.x, s3.y);

}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'd') position.x += 15;
	if (k == 'a') position.x -= 15;
	if (k == 'w') position.y += 15;
	if (k == 's') position.y -= 15;
}

void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

#endif _MAIN_