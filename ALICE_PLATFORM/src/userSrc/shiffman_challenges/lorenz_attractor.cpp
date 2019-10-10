//#define _MAIN_

#ifdef _MAIN_

// Lorenz Attractor
// https ://en.wikipedia.org/wiki/Lorenz_system

#include "main.h"

using namespace std;

double rotation_angle;
double rotation_speed = 0.3;

float scale = 1.5;

double x = 0.01;
double y = 0;
double z = 0;

double a = 10; // sigma
double b = 28; // beta
double c = 8.0 / 3.0; // rho

vector<vec> points;


void setup()
{
	setCamera(100, -100, 0, 0, -40);
}

void update(int value)
{
	// rotating object
	rotation_angle += rotation_speed;

	// lorenz formular
	double dt = 0.01;
	double dx = (a * (y - x)) * dt;
	double dy = (x * (b - z) - y) * dt;
	double dz = (x * y - c * z) * dt;
	x = x + dx;
	y = y + dy;
	z = z + dz;

	// collecting all points in a vector
	points.push_back(vec(x, y, z) * scale);
}

void draw()
{
	backGround(0.75);
	updateCam = false;

	glPushMatrix();
	glRotated(rotation_angle, 0, 0, 1);
	glColor3d(1, 0, 0.55);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points.size(); i++)
		glVertex3d(points[i].x, points[i].y, points[i].z);
	glEnd();
	glPopMatrix();
}

void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 's')
		rotation_speed = 0;
	if (k == 'S')
		rotation_speed = 0.3;
}

void mousePress(int b, int s, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif // _MAIN_