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

double cWidth = 10;
double cLenght = 20;
double cHeight = 10;
vec cCenter(20, 0, 20);

bool showGeometry = true;

///////// ----------------------------------------- model - view - controller (MVC) paradigm / pattern / template  ----------------- ////////////////////////////// 
/////////////////////////// model  ///////////////////////////////////////////
void setup()
{
	S = *new SliderGroup();
	S.addSlider(&cWidth, "Width of cube");
	S.addSlider(&cLenght, "Length of cube");
	S.addSlider(&cHeight, "Height of cube");

	S.sliders[0].attachToVariable(&cWidth, 0, 100);
	S.sliders[1].attachToVariable(&cLenght, 0, 100);
	S.sliders[2].attachToVariable(&cHeight, 0, 100);


	//somehow the button is not working
	B = *new ButtonGroup(vec(50, 450, 0));
	B.addButton(&showGeometry, "showGeometry");
}

void update(int value)
{

}

/////////////////////////// view  ///////////////////////////////////////////
void draw()
{
		backGround(0.75);
		drawGrid(20);
		

		if (showGeometry)
		{
			for (int i = 0; i < 5; i++)
			{
				glPushMatrix();
				glTranslatef(0.0, 0.0, cHeight * i + (cHeight*0.5));
				wireFrameOn();
				glColor3f(1, 1, 1);
				drawCube(vec(cWidth * -0.5, cLenght * -0.5, cHeight * -0.5) + cCenter, vec(cWidth * 0.5, cLenght * 0.5, cHeight * 0.5) + cCenter);
				wireFrameOff();
				glPopMatrix();
			}
		}
}

/////////////////////////// control  ///////////////////////////////////////////
void keyPress(unsigned char k, int xm, int ym)
{

}

void mousePress(int b, int state, int x, int y)
{
	if (GLUT_LEFT_BUTTON == b && GLUT_DOWN == state)
	{
		S.performSelection(x, y, HUDSelectOn);
		B.performSelection(x, y);
	}
}

void mouseMotion(int x, int y)
{
	S.performSelection(x, y, HUDSelectOn);
}
#endif // _MAIN_