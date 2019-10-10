//#define _MAIN_

#ifdef _MAIN_

#include "main.h"
using namespace std;

float a = 60;
float b = 60;
double n = 4;

int sgn(float val)
{
	if (val > 0)
		return 1;
	else if (val < 0)
		return -1;
	else
		return 0;
}

void setup()
{
	S = *new SliderGroup();
	S.addSlider(&n, " ");
	S.sliders[0].attachToVariable(&n, 0.1, 10);
}

void update(int value)
{
}

void draw()
{
	backGround(0.75);
	updateCam = false;

	glBegin(GL_LINE_LOOP);
	for (float angle = 0; angle < TWO_PI; angle += 0.1)
	{
		float na = 2 / n;
		float x = pow(abs(cos(angle)), na) * a * sgn(cos(angle));
		float y = pow(abs(sin(angle)), na) * b * sgn(sin(angle));
		glVertex2d(x, y);
	}
	glEnd();

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