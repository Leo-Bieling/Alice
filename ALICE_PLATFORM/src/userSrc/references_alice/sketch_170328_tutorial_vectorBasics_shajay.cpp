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
	
	vec n(5, 5, 5);
	vec cen(0, 10, 0);
	vec u = n.cross(vec(1, 0, 0));
	vec v = n.cross(u);

	u.normalise();
	v.normalise();
	n.normalise();

	float w = 3.0;

	vec A = u * w + v * (-w);
	vec B = u * (-w) + v * (-w);
	vec C = u * (-w) + v * (w);
	vec D = u * (w) +v * (w);

	A += cen;
	B += cen;
	C += cen;
	D += cen;

	drawLine(cen, cen + n * 3);
	drawCircle(cen, 0.5, 32);

	glColor3f(1, 0, 0);
	drawLine(cen, cen + u *3);

	glColor3f(0, 1, 0);
	drawLine(cen, cen + v *3);

	glColor3f(1, 0, 1);
	glLineWidth(4);
	drawLine(A, B);
	drawLine(B, C);
	drawLine(C, D);
	drawLine(D, A);
	glLineWidth(1);

	drawPlane(n, cen);
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