//#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zData.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zIO.h>


using namespace zSpace;
using namespace std;


float total = 2000;
float n;
float c = 2;
float angle_const = 137.3;
vector<zVector> collection_of_positions;
vector<zColor> collection_of_colors;

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// setting up camera for rotation
	setCamera(200, -50, 50, 0, -20);

	n = total;
}

void update(int value)
{
	float a = n * angle_const;
	float r = c * sqrtf(n);

	
	if (n > -1.0)
	{
		// translate polar system (radius and angle) to kartesian system (x and y) 
		zVector position;
		position.x = r * cos(a);
		position.y = r * sin(a);
		position.z = (total - n) * 0.1;
		collection_of_positions.push_back(position);

		// setting up the color change
		zColor color;
		color.r = ((1 / total) * n);
		collection_of_colors.push_back(color);
		
		n--;
	}
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	setup2d();
	drawString("Press 1 for 137.3", 50, 100);
	drawString("Press 2 for 137.5", 50, 125);
	drawString("Press 2 for 137.6", 50, 150);
	restore3d();

	//for (auto p : collection_of_positions) drawPoint(p, zColor(1, 0, 0, 1), 4);

	for (int i = 0; i < collection_of_positions.size(); i++)
	{
		drawPoint(collection_of_positions[i], collection_of_colors[i], 6);
		drawLine(collection_of_positions[i], collection_of_positions[i + 1]);
	}
}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == '1')
	{
		collection_of_positions.clear();
		collection_of_colors.clear();
		n = total;
		angle_const = 137.3;
	}

	if (k == '2')
	{
		collection_of_positions.clear();
		collection_of_colors.clear();
		n = total;
		angle_const = 137.5;
	}

	if (k == '3')
	{
		collection_of_positions.clear();
		collection_of_colors.clear();
		n = total;
		angle_const = 137.6;
	}

}

void mousePress(int b, int state, int x, int y)
{
	
}

void mouseMotion(int x, int y)
{
	
}

#endif _MAIN_