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

vec planeOrigin = vec(8, 5, 12);
int scale = 15;

///////// ----------------------------------------- model - view - controller (MVC) paradigm / pattern / template  ----------------- ////////////////////////////// 
/////////////////////////// model  ///////////////////////////////////////////
void setup()
{
	
}

void update(int value)
{
	planeOrigin.x += planeOrigin.y * 0.02;
	planeOrigin.y += planeOrigin.x * -0.02;
}
/////////////////////////// view  ///////////////////////////////////////////
void draw()
{
	backGround(0.75);
	drawGrid(20);

	vec N = planeOrigin + vec(0.75, 0.2, 0); //I think that is wrong...
	vec U = N.cross(vec(1, 0, 0));
	vec V = N.cross(U);
	vec unit_U = U / U.mag();
	vec unit_V = V / V.mag();

	glPointSize(5);
	glColor3f(0, 1, 0);
	drawPoint(N);
	drawLine(N, planeOrigin);
	glColor3f(0, 0, 1);
	drawPoint(planeOrigin);

	vec scaled_U = unit_U * scale;
	vec scaled_V = unit_V * scale;

	vec A = (scaled_U + scaled_V) + planeOrigin;
	vec B = (scaled_U - scaled_V) + planeOrigin;
	vec C = ((scaled_U*-1) - scaled_V) + planeOrigin;
	vec D = ((scaled_U*-1) + scaled_V) + planeOrigin;

	glColor3f(0, 0, 0);
	drawLine(A, B);
	drawLine(B, C);
	drawLine(C, D);
	drawLine(D, A);

	for (int i = -scale; i < scale; i++)
	{
		vec A1 = (unit_U * i + scaled_V) + planeOrigin;
		vec B1 = (unit_U * i - scaled_V) + planeOrigin;
		vec C1 = ((scaled_U)-unit_V * -i) + planeOrigin;
		vec D1 = ((scaled_U*-1) + unit_V * i) + planeOrigin;
		glColor3f(1, 0, 0);
		drawLine(A1, B1);
		drawLine(C1, D1);
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

}
#endif // _MAIN_