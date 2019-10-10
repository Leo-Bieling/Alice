#define _MAIN_

#ifdef _MAIN_
#include "main.h"
#include "ALICE_ROBOT_DLL.h"
using namespace ROBOTICS;
#include <array>
#include <memory>
#include <time.h>
#include <experimental/generator> 

using namespace std;
using namespace std::experimental;

// model - view - controller (MVC) paradigm / pattern / template 
vec P(1, 5, 0);
vec V(0.5, -1, 0);
vec A(-8, 0, 0);
vec B(8, 0, 0);

void setup()
{
	P = vec(1, 5, 0);
	V = vec(0.5, -1, 0);
	A = vec(-8, 0, 0);
	B = vec(8, 0, 0);
}

void update(int value)
{
	vec V_unit = V / V.mag();

	if (P.mag() > 2)
	{
		P = P + V_unit * 0.1;
	}

	else
	{
		vec unit_normal = P / P.mag();
		vec unit_normal_negative = unit_normal * -1;

		vec Vn, Vt;
		float lenghtOfVn = V * unit_normal_negative;

		Vn = unit_normal_negative * lenghtOfVn;
		Vt = V - Vn;

		vec X = Vt + (Vn * -1);
		V = X;
		P = unit_normal * 1.01;
	}
}

void draw()
{
	backGround(0.75);	
	glPointSize(4);
	drawPoint(P);
	drawLine(P, P + V);

	glPointSize(4);
	glColor3f(1, 0, 0);
	vec A(-8, 1, 0);
	vec B(8, 1, 0);
	drawLine(A, B);
}

void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'r')setup();
}
#endif
