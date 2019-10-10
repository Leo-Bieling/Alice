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

// curve class
class Curve
{
public:
	vector<zVector> path;
	zVector current;
	Curve() {}
	~Curve() {}
	
	void setX(float x)
	{
		current.x = x;
	}

	void setY(float y)
	{
		current.y = y;
	}

	void addPoint()
	{
		path.push_back(current);
	}

	void show()
	{
		glColor3d(1, 0, 0.55);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < path.size(); i++)
			glVertex3d(path[i].x, path[i].y, path[i].z);
		glEnd();
	}

	void reset()
	{
		path.clear();
	}
private:

};

bool reset = false;
const int WIDTH = 150;
const int HEIGHT = 150;
const int W = 15;
const int COLS = WIDTH / W - 1;
const int ROWS = HEIGHT / W - 1;
float angle = 0;
float d = W - W / 4;
float r = d / 2;

Curve curves[ROWS][COLS];

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// enable smooth display
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);

	B = *new ButtonGroup();
	B.addButton(&reset, "Reset");
}

void update(int value)
{
	angle -= 0.005;

	if (reset)
	{
		for (int j = 0; j < ROWS; j++)
			for (int i = 0; i < COLS; i++)
			{
				curves[j][i].reset();
			}
		reset = !reset;
	}
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(1.0);
	drawGrid(75.0);

	// cols
	for (int i = 0; i < COLS; i++)
	{
		float cx = W + i * W + W / 2 - WIDTH / 2;
		float cy = -WIDTH / 2;
		drawCircle(zVector(cx, cy, 0), 32, d/2);

		float x = r * cos(angle * (i + 1) + HALF_PI);
		float y = r * sin(angle * (i + 1) + HALF_PI);

		drawPoint(zVector(cx + x, cy + y, 0), zColor(1, 0, (100/ 255), 1), 6.0);
		drawLine(zVector(cx + x, HEIGHT / 2, 0), zVector(cx + x, -HEIGHT / 2, 0), zColor(0.75, 0.75, 0.75, 0.25), 1.0);
		
		for (int j = 0; j < ROWS; j++)
			curves[j][i].setX(cx + x);
	}

	// rows
	for (int j = 0; j < ROWS; j++)
	{
		float cx = -HEIGHT / 2;
		float cy = W + j * W + W / 2 - WIDTH / 2;
		drawCircle(zVector(cx, cy, 0), 32, d / 2);

		float x = r * cos(angle * (j + 1) + PI / 2);
		float y = r * sin(angle * (j + 1) + PI / 2);

		drawPoint(zVector(cx + x, cy + y, 0), zColor(1, 0, (100 / 255), 1), 6.0);
		drawLine(zVector(-WIDTH / 2, cy + y , 0), zVector(WIDTH / 2, cy + y, 0), zColor(0.75, 0.75, 0.75, 0.25), 1.0);
		
		for (int i = 0; i < COLS; i++)
			curves[j][i].setY(cy + y);
	}

	for (int j = 0; j < ROWS; j++)
		for (int i = 0; i < COLS; i++)
		{
			curves[j][i].addPoint();
			curves[j][i].show();
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