//#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zData.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zIO.h>

#include <userSrc/shiffman_challenges/circle_packing_circle.h>

using namespace zSpace;
using namespace std;

Circle circle;
vector<Circle> collection_of_circles;

void newCircle()
{
	zVector tmp_position;
	tmp_position.x = randomNumber(-40, 40);
	tmp_position.y = randomNumber(-40, 40);

	bool valid = true;
	for (auto &c : collection_of_circles)
	{
		float d = tmp_position.distanceTo(c.position);
		if (d < c.radius)
		{
			valid = false;
			break;
		}
	}

	if (valid)
	{
		collection_of_circles.push_back(circle = Circle(tmp_position));
	}
}
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{

}

void update(int value)
{
	newCircle();
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(40.0);

	for (Circle &c : collection_of_circles)
	{
		if (c.growing)
		{
			if (c.edges())
			{
				c.growing = false;
			}
			else
			{
				for (Circle &other : collection_of_circles)
				{
					if (c.position.x != other.position.x && c.position.y != other.position.y)
					{
						float d = c.position.distanceTo(other.position);
						if (d < (c.radius + other.radius))
						{
							c.growing = false;
							break;
						}
					}
				}
			}

		}
		c.draw();
		c.grow();
	}
}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{

}

void mousePress(int b, int state, int x, int y)
{
	
}

void mouseMotion(int x, int y)
{
	
}



#endif _MAIN_