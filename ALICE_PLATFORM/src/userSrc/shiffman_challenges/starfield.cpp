//#define _MAIN_

#ifdef _MAIN_

#include "main.h"
#include <zSpaceImplement.h>
#include <zDisplay.h>

using namespace zSpace;
using namespace std;



double speed = 0.1;

class star
{
public:

	double x, y, z, pz;
	int width = 100;
	int height = 100;


	void setup()
	{
		x = randomNumber_double(-width, width);
		y = randomNumber_double(-height, height);
		z = randomNumber_double(0, width);
		pz = z;
	}

	void update()
	{
		z = z - speed;

		if (z < 1)
		{
			x = randomNumber_double(-width, width);
			y = randomNumber_double(-height, height);
			z = width;
			pz = z;
		}
	}

	void draw()
	{
		// low2 + (value - low1) * (high2 - low2) / (high1 - low1)
		double sx = 0 + ((x / z) - 0) * (width - 0) / (1 - 0); // mapping (x/z) to 0-1 from 0-width
		double sy = 0 + ((y / z) - 0) * (height - 0) / (1 - 0);; // mapping (x/z) to 0-1 from 0-height

		double r = 16 + (z - 0) * (0 - 16) / (width - 16);

		drawPoint(zVector(sx, sy, z), zColor(1, 1, 1, 1), r);

		double px = 0 + ((x / pz) - 0) * (width - 0) / (1 - 0);
		double py = 0 + ((y / pz) - 0) * (height - 0) / (1 - 0);

		pz = z;

		//drawLine(zVector(px, py, 0), zVector(sx, sy, 0), zColor(1, 1, 1, 1), r);
	}
};

vector<star> COS;
int total_stars = 5000;

void setup()
{


	S = *new SliderGroup();
	S.addSlider(&speed, "Speed");
	S.sliders[0].attachToVariable(&speed, 0, 4);
	

	for (int i = 0; i < total_stars; i++)
	{
		star a;
		COS.push_back(a);
	}

	for (auto &star : COS)star.setup();
}

void update(int value)
{
	for (auto &str : COS)str.update();
}

void draw()
{
	backGround(0.05);

	for (auto &str : COS)str.draw();
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