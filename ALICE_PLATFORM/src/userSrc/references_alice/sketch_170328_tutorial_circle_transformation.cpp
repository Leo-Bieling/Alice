#define _MAIN_

#ifdef _MAIN_

#include "main.h"
#include "MODEL.h"
#include <array>
#include <memory>
#include <time.h>
#include <experimental/generator> 
using namespace std;
using namespace std::experimental;



///////// ----------------------------------------- model - view - controller (MVC) paradigm / pattern / template  ----------------- ////////////////////////////// 
/////////////////////////// model  ///////////////////////////////////////////

vec u(1, 1, 1);

void setup()
{

}

void update(int value)
{

}

/////////////////////////// view  ///////////////////////////////////////////

void draw()
{
	backGround(0.75);
	drawGrid(20);

	//construct the circle
	float radius = 18;
	int numPoints = 32;
	float angle = TWO_PI / (float)numPoints;
	glColor3f(1, 0.6, 1);
	glPointSize(4);
	for (int i = 0; i < numPoints; i++)
	{
		drawLine(vec(radius*sin(angle*i), radius*cos(angle*i), 0), vec(radius*sin(angle*(i - 1)), radius*cos(angle*(i - 1)), 0));
		drawPoint(vec(radius*sin(angle*i), radius*cos(angle*i), 0));
	}

	//given u, v, n, basis vector of tranformes space
	vec v = u.cross(vec(0, 0, 1));
	vec n = u.cross(v);
	u.normalise();
	v.normalise();
	n.normalise();

	vec origin(0, 0, 0);
	vec newOrigin(20, 5, 20);

	glColor3f(0, 0, 0);
	drawLine(origin, newOrigin);

	glColor3f(1, 0, 0);
	drawLine(origin + newOrigin, u * 2 + newOrigin);
	glColor3f(0, 1, 0);
	drawLine(origin + newOrigin, v * 2 + newOrigin);
	glColor3f(0, 0, 1);
	drawLine(origin + newOrigin, n * 2 + newOrigin);

	//construct matrix
	Matrix3 rotMatrix;
	rotMatrix.setColumn(0, u);
	rotMatrix.setColumn(1, v);
	rotMatrix.setColumn(2, n);

	//draw translated circle
	glColor3f(1, 0, 1);
	for (int i = 0; i < numPoints; i++)
	{
		drawLine((rotMatrix * vec(radius*sin(angle*i), radius*cos(angle*i), 0) + newOrigin), (rotMatrix *vec(radius*sin(angle*(i - 1)), radius*cos(angle*(i - 1)), 0) + newOrigin));
		drawPoint(rotMatrix *vec(radius*sin(angle*i), radius*cos(angle*i), 0) + newOrigin);
	}
}

/////////////////////////// control  ///////////////////////////////////////////

void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'p')
	{
		u.x += u.y * 0.05;
		u.y += u.x * -0.05;
		u.z += u.y * 0.05;
	}
}
#endif // _MAIN_