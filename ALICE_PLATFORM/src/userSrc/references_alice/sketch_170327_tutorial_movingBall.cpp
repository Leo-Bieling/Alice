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
vec P;
vec V = vec(0.25, .764, 0);

void setup()
{

}

void update(int value)
{
	if (P.x >= 20 || P.x <= -20) V.x *= -1;
	if (P.y >= 20 || P.y <= -20) V.y *= -1;

	P += V;
}

/////////////////////////// view  ///////////////////////////////////////////

void draw()
{
	backGround(0.75);
	drawGrid(20);
	
	glPointSize(10);
	glColor3f(1, 0, 0);
	drawPoint(P);
	
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