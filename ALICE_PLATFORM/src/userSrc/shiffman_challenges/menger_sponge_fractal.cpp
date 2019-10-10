//#define _MAIN_

#ifdef _MAIN_

#include "main.h"
#include <vector>
#include <challenges/menger_sponge_fractal_box.h>
using namespace std;

double rotation_speed = 0.2;
vector<Box> sponge;
double rotation_angle;

void setup()
{
	Box b = Box(0, 0, 0, 40);
	sponge.push_back(b);

	GLfloat light[] = { 1.0, 1.0, 1.0, 1.0 };	// light direction
	enableLight(light);							// enabeling light
}

void update(int value)
{
	
}

void draw()
{
	backGround(0.75);
	glPushMatrix();
	glRotated(rotation_angle, 0, 0, 1);
	for (auto &bxs : sponge) bxs.draw();
	glPopMatrix();

	rotation_angle += rotation_speed;
}

void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'b')
	{
		vector<Box> next;
		for (auto &b : sponge)
		{
			vector<Box> new_boxes = b.generate();
			for (auto &bs : new_boxes)next.push_back(bs);
		}
		sponge = next;

		cout << "The sketch is displaying " << sponge.size() << " boxes." << endl;
	}
}

void mousePress(int b, int s, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

#endif // _MAIN_