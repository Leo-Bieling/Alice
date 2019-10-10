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

	backGround(0.75);
	drawGrid(20);

	glBegin(GL_QUADS);
	glColor3f(0.5, 0.0, 1.0); // make this vertex purple
	glVertex2f(-10, 10);
	glColor3f(1.0, 0.0, 0.0); // make this vertex red
	glVertex2f(-10, -10);
	glColor3f(0.0, 1.0, 0.0); // make this vertex green
	glVertex2f(10, -10);
	glColor3f(1.0, 1.0, 0.0); // make this vertex yellow
	glVertex2f(10, 10);
	glEnd();


}

/////////////////////////// control  ///////////////////////////////////////////
void keyPress(unsigned char k, int xm, int ym)
{

}

void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}
#endif // _MAIN_