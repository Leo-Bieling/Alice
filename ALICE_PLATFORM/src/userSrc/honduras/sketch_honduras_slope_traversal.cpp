#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zData.h>
#include <headers/include/zIO.h>

using namespace zSpace;
using namespace std;



////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
zMesh terrain;
zBufferObject buffer_obj;
vector<zVector> face_centers;

////// --- GUI OBJECTS ----------------------------------------------------
bool smooth = false;
bool face_normals = false;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// enable smooth display
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);

	// import mesh
	fromOBJ(terrain, "data/turtlingBayPatch.obj");
	
	// create buffer
	buffer_obj = zBufferObject(1000000);

	// send initial mesh to buffer
	buffer_obj.appendMesh(terrain);

	// initialize buttonGroup
	B = *new ButtonGroup();
	B.addButton(&smooth, "smooth");
	B.addButton(&face_normals, "face_normals");

	////////////////////////////////////////////////////////////////////////// Sliders

	//S = *new SliderGroup();

	//S.addSlider(&background, "background");
	//S.sliders[0].attachToVariable(&background, 0, 1);

	//////////////////////////////////////////////////////////////////////////// Buttons

	//B = *new ButtonGroup(vec(50, 450, 0));

	//B.addButton(&compute, "compute");
	//B.buttons[0].attachToVariable(&compute);
	//B.addButton(&display, "display");
	//B.buttons[1].attachToVariable(&display);


}

void update(int value)
{
	if (smooth)
	{
		// smooth mesh
		smoothMesh(terrain);

		// flip the bool back to false
		smooth = !smooth;

		// clear buffer for new mesh
		buffer_obj.clearBufferForRewrite();

		// append new mesh to buffer
		buffer_obj.appendMesh(terrain);
	}

	// compute face centers for displaying the face normals
	if (face_normals) getCenters(terrain, zFaceData, face_centers);
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	// draw mesh from the buffer
	drawLinesFromBuffer(buffer_obj, false);
	drawQuadsFromBuffer(buffer_obj, true);

	// draw face normals
	if (face_normals) drawMesh_FaceNormals(terrain, face_centers, 2.0);

	// draw buttons
	B.draw();

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

#endif // _MAIN_