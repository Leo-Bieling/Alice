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


// return-type nameOfFunction ( arguments )
void divideSegment_and_snapToCircle(vec A, vec B, int numdivisions, float r, vec cen, vec N)
{
	//construct new frame (u,v,n) from normal ; N is given
	vec U = N.cross(vec(1, 0, 0));
	vec V = U.cross(N);
	U.normalise();
	V.normalise();
	N.normalise();
	// construct matrix (rotMatrix) using U,V,N ;
	Matrix3 rotmatrix;
	rotmatrix.setColumn(0, U);
	rotmatrix.setColumn(1, V);
	rotmatrix.setColumn(2, N);


	//
	vec ed = B - A;
	float dis = ed.mag();
	ed.normalise();

	for (int i = 0; i < numdivisions; i++)
	{

		vec P = ed * (dis / numdivisions) * i;
		P += A;

		vec PUnit = P / P.mag();
		vec PonCircle = PUnit * r;

		glPointSize(5);
		glColor3f(1, 0, 0);
		drawPoint(P);
		drawPoint(PonCircle);

		vec PonCircle_onThePlane = rotmatrix * PonCircle + cen;
		drawPoint(PonCircle_onThePlane);

	}
}

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

	vec normal = vec(5, 5, 5);
	vec center = vec(3, 4, 5);

	wireFrameOn();

	drawPlane(normal, center);

	wireFrameOff();

	// step 1 : draw a circle ;

	// draw the four quadrant points ;
	float r = 3;
	vec A, B, C, D;
	A = vec(r, 0, 0); C = A * -1;
	B = vec(0, r, 0); D = B * -1;

	drawLine(A, B);
	drawLine(B, C);
	drawLine(C, D);
	drawLine(D, A);

	//divide segment A to B into 5 segments;

	divideSegment_and_snapToCircle(A, B, 20, r, center, normal);
	divideSegment_and_snapToCircle(B, C, 20, r, center, normal);
	divideSegment_and_snapToCircle(C, D, 20, r, center, normal);
	divideSegment_and_snapToCircle(D, A, 20, r, center, normal);


	// draw a circle with equation of a circle ;
	int numDivs = 32;
	float angularIncrement = PI * 2.0 / numDivs; // in radians
	for (int i = 0; i < 32; i++)
	{
		float xCoord = r * cos(angularIncrement*i);
		float yCoord = r * sin(angularIncrement*i);
		drawPoint(vec(xCoord, yCoord, 0));
	}


	//draw text dots
	drawString("A", A);
	drawString("B", B);
	drawString("C", C);
	drawString("D", D);

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



