#include "main.h"
#include "ALICE_ROBOT_DLL.h"
using namespace ROBOTICS;
#include <array>
#include <memory>
#include<time.h>
#include<experimental/generator> 

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
		vec unitC = (B - A) / (B - A).mag();// *.05;
		float totalDistance = E.mag();
		float incrementalDistance = (totalDistance / 6.0) * i;
		temp = A + unitC * incrementalDistance;
		collectionOfPoints.push_back(temp);

	}

	for (int i = 0; i < 6; i++)
	{
		vec E = C - B;
		vec unitC = (C - B) / (C - B).mag();// *.05;
		float totalDistance = E.mag();
		float incrementalDistance = (totalDistance / 6.0) * i;
		temp = B + unitC * incrementalDistance;
		collectionOfPoints.push_back(temp);
	}

	for (int i = 0; i < 6; i++)
	{
		vec E = D - C;
		vec unitC = (D - C) / (D - C).mag();// *.05;
		float totalDistance = E.mag();
		float incrementalDistance = (totalDistance / 6.0) * i;
		temp = C + unitC * incrementalDistance;
		collectionOfPoints.push_back(temp);

	}

	for (int i = 0; i < 6; i++)
	{
		vec E = C - D;
		vec unitC = (C - D) / (C - D).mag(); //* .05;
		float totalDistance = E.mag();
		float incrementalDistance = (totalDistance / 6.0) * i;
		temp = D + unitC * incrementalDistance;
		collectionOfPoints.push_back(temp);

	}
	


}


void update(int value)
{
	

	
}



void draw()
{
	backGround(.75);
	glPointSize(5);
	drawPoint(A);
	drawPoint(B);
	drawPoint(C);
	drawPoint(D);
	
	glColor3f(1, 0, 0);
	for (auto &pt : collectionOfPoints)
	{
		drawPoint(pt);
	}
	
}
void keyPress(unsigned char k, int xm, int ym)
{
	for (int i = 1; i < collectionOfPoints.size()-1; i++)
	{
	
		int idofNborFront = i + 1;
		int idofNborBack = i - 1;
		vec Pf = collectionOfPoints[idofNborFront];
		vec Pb = collectionOfPoints[idofNborBack];
		vec A = collectionOfPoints[i];
		vec Avg = (Pf + Pb) * 0.5; //change to smooth to the outside .9
		vec Diff = Avg - A;
		vec unitDiff = Diff / Diff.mag();
		if (unitDiff.mag() < 0.1)continue;
		A = A + Diff //* 0.01; 
		collectionOfPoints[i] = A;
	}

	
}



void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}




