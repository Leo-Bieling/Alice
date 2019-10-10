//#define _MAIN_

#ifdef _MAIN_

#include "main.h"
#include <challenges/snake.h>
using namespace std;


Snake s;
double playground_x = 200;
double playground_y = 150;
double scl = 5;
double speed = 0.2;
vec food;


void pick_location()
{
	double cols = playground_x / scl;
	double rows = playground_y / scl;
	food = vec(ofRandom(-(cols / 2)*scl, (cols / 2)*scl), ofRandom(-(rows / 2)*scl, (rows / 2)*scl), 0);
}


////////////////////////////////////////////////////////////////////////////

void setup()
{
	// initialise snake
	Snake();
	// initial location of food
	pick_location();
}


void update(int value)
{
	s.death();
	// update snake
	s.update(playground_x, playground_y, scl);
}

void draw()
{
	backGround(0);
	// make camera static
	updateCam = false;

	// draw playgound
	wireFrameOn();
	drawRectangle(vec(-playground_x / 2 - s.scl / 2, -playground_y / 2 - s.scl / 2, 0), vec(playground_x / 2 + s.scl / 2, playground_y / 2 + s.scl / 2, 0));
	wireFrameOff();

	// draw snake
	s.draw();
	if (s.eat(food)) pick_location();

	// draw food
	drawRectangle(vec(food.x - (scl / 2), food.y - (scl / 2), 0), vec(food.x + (scl / 2), food.y + (scl / 2), 0));
}

void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 's') s.dir(0, -speed);
	else if (k == 'w') s.dir(0, speed);
	else if (k == 'd') s.dir(speed, 0);
	else if (k == 'a') s.dir(-speed, 0);
}

void mousePress(int b, int s, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif // _MAIN_
