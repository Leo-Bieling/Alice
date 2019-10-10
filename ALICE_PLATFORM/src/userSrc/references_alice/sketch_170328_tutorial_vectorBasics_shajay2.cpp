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
	double ht = 350;
	double th_plus = 20;

	backGround(0.75);
	drawGrid(20);

	//given input
	vec cen(5, 5, 5); //new origin
	vec n(1, 1, 1); //normal axis
	vec P(4, 3, 4); //any point in global axes

	//visualise input
	drawPoint(cen); // new origin
	drawLine(cen, cen + n); // draw noraml axis
	drawPoint(P); //draw point P
	drawPlane(n, cen);

	//step 1 - substract new origin from P
	P -= cen;
	drawLine(cen, cen + P);

	//step 2 - get u and v axes
	vec u = n.cross(vec(1, 0, 0));
	vec v = n.cross(u);
	u.normalise();
	v.normalise();
	n.normalise();

	//step 3 - project the new vestor (w.r.t. new axis) unto the new axis
	//get the u,v,n coordinates
	float uCoord = u * P;
	float vCoord = v * P;
	float nCoord = n * P;

	//cheching if new coordindate and axes correspond with old coordinates and axis;
	vec checkPt = (u * uCoord) + (v * vCoord) + (n * nCoord);
	checkPt += cen;
	drawCircle(checkPt, 0.5, 32);

	//step 4 = preojection;
	vec projPt = u * uCoord + v * vCoord;
	glColor3f(1, 0, 0);
	glLineWidth(4);
	drawLine(projPt + cen, P + cen);
	glLineWidth(1);

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