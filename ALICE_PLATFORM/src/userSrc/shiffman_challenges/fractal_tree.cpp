//#define _MAIN_

#ifdef _MAIN_

#include "main.h"

using namespace std;

double length = 80;
double angle = 45;
double factor = 0.67;

double rotation_angle;
double rotation_speed = 0.3;

void branch(int len)
{
	glLineWidth(2);
	glColor3d(1, 0, 0.55);
	drawLine(vec(0, 0, 0), vec(0, 0, len));

	if (len > 5)
	{
		glPushMatrix();
		glTranslatef(0, 0, len);
		glRotatef(-angle, 0, 1, 0);
		branch(len * factor);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 0, len);
		glRotatef(angle, 0, 1, 0);
		branch(len * factor);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 0, len);
		glRotatef(-angle, 1, 0, 0);
		branch(len * factor);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 0, len);
		glRotatef(angle, 1, 0, 0);
		branch(len * factor);
		glPopMatrix();
	}
}

void setup()
{
	S = *new SliderGroup();
	S.addSlider(&length, "Length");
	S.sliders[0].attachToVariable(&length, 5, 100);
	S.addSlider(&angle, "Angle");
	S.sliders[1].attachToVariable(&angle, 0, 100);
	S.addSlider(&factor, "Factor");
	S.sliders[2].attachToVariable(&factor, 0.2, 0.98);

	setCamera(300, -100, 100, 0, -70);
}

void update(int value)
{
	// rotating object
	rotation_angle += rotation_speed;
}

void draw()
{
	backGround(0.75);
	updateCam = false;

	glPushMatrix();
	glRotated(rotation_angle, 0, 0, 1);
	branch(length);
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