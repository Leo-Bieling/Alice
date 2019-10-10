//#define _MAIN_

#ifdef _MAIN_

#include "main.h"
using namespace std;

double rotation_angle;
double rotation_speed = 0.3;

float x;
float y;
float z;
vector<vec> COP;

void setup()
{
	setCamera(100, -100, 0, 0, 0);
}

void update(int value)
{
	// rotating object
	rotation_angle += rotation_speed;

	// collect random points
	int r = floor(ofRandom(0.0, 6.0));

	switch (r)
	{
	case 0:
		x = x + 1;
		break;
	case 1:
		x = x - 1;
		break;
	case 2:
		y = y + 1;
		break;
	case 3:
		y = y - 1;
		break;
	case 4:
		z = z + 1;
		break;
	case 5:
		z = z - 1;
		break;
	default:
		break;
	}

	COP.push_back(vec(x, y, z));
}

void draw()
{
	backGround(0.75);
	updateCam = false;

	glPushMatrix();
	glRotated(rotation_angle, 0, 0, 1);
	glPointSize(4);
	glColor3d(1, 0, 0.55);
	for (auto &pt : COP) drawPoint(pt);
	glPopMatrix();
}

void keyPress(unsigned char k, int xm, int ym)
{
}

void mousePress(int b, int s, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif // _MAIN_