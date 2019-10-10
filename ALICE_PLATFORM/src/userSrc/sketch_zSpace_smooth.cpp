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

zMesh myMesh;
zBufferObject Buffer;
bool smooth;

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// initialize buttons
	B = *new ButtonGroup(vec(50, 500, 0));

	// assign and lable button
	B.addButton(&smooth, "smooth");

	// create buffer
	Buffer = zBufferObject(100000);

	// import obj
	fromOBJ(myMesh, "data/zSpace_smooth_test_2.obj");

	// add mesh to buffer
	Buffer.appendMesh(myMesh);
}

void update(int value)
{
	
	if (smooth)
	{
		// smooth mesh
		smoothMesh(myMesh, 1, true);

		// clear buffer for edited mesh
		Buffer.clearBufferForRewrite();

		// add edited mesh to buffer
		Buffer.appendMesh(myMesh);

		// flip smooth to false
		smooth = !smooth;
	}

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	// draw button
	B.draw();

	// draw mesh from buffer
	drawLinesFromBuffer(Buffer, false);
	drawPointsFromBuffer(Buffer);
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