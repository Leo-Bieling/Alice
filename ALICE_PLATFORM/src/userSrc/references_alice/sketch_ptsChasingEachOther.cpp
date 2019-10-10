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

vec A = vec (1, 1, 0);
vec B = vec (1, -1, 0);
vec C = vec (-1, -1, 0);
vec D = vec(-1, 1, 0);
vec temp;

vector<vec>collectionOfPoints;

void setup()
{
	for (int i = 0; i < 6; i++)
	{
		vec E = B - A;
		vec unitC = (B - A) / (B - A).mag();
		float totalDistance = E.mag();
		float incrementalDistance = (totalDistance / 6.0) * i;
		temp = A + unitC * incrementalDistance;
	}

	for (int i = 0; i < 6; i++)
	{
		vec E = C - B;
		vec unitC = (C - B) / (C - B).mag();
		float totalDistance = E.mag();
		float incrementalDistance = (totalDistance / 6.0) * i;
		temp = B + unitC * incrementalDistance;
	}

	for (int i = 0; i < 6; i++)
	{
		vec E = D - C;
		vec unitC = (D - C) / (D - C).mag();
		float totalDistance = E.mag();
		float incrementalDistance = (totalDistance / 6.0) * i;
		temp = C + unitC * incrementalDistance;
	}

	collectionOfPoints.push_back(tmp);
}


void update(int value)
{
	drawPoint(A);
	drawPoint(B);
	drawPoint(C);
	drawPoint(D);
	drawLine(temp, A);

	for (auto &pt : collectionOfPoints)
	{
		draw(pt);
	}
}

void draw()
{

}

void keyPress(unsigned char k, int xm, int ym)
{
	for (int i = 1; i < collectionOfPoints.size() - 1; i++)
	{
	 
		int idofNborFront = i + 1;
		int idofNborBack = i - 1;
		vec Pf = collectionOfPoints[idofNborFront];
		vec PB = collectionOfPoints[idofNborBack];
		vec A = collectionOfPoints[i];
		vec Avg = (Pf + Pb) * 0.5;
		vec Diff = Avg - A;
		vec unitDiff = Diff / Diff.mag();
		if (unitDiff.mag() < 0.001)continue;
		A = A + unitDiff * 0.1;

		collectionOfPoints [i] = A
	}
}

void mousePress(int b, int state, int x, int y)
{
	
}

void mouseMotion(int x, int y)
{

}
#endif
