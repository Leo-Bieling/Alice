//#define _MAIN_

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

zMesh myMesh;
zBufferObject buffer_obj;

////// --- GUI OBJECTS ----------------------------------------------------
// bool for buttons
bool smooth = false;
bool face_normals = false;
vector<zVector> face_centers;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// import obj
	fromOBJ(myMesh, "data/zSpace_smooth_test.obj");
	
	// create buffer
	buffer_obj = zBufferObject(1000000);

	// send initial mesh to buffer
	buffer_obj.appendMesh(myMesh);

	// initialize buttonGroup
	B = *new ButtonGroup();
	B.addButton(&smooth, "smooth");
	B.addButton(&face_normals, "face_normals");
}

void update(int value)
{	
	if (smooth)
	{
		// smooth mesh
		smoothMesh(myMesh);

		// flip the bool back to false
		smooth = !smooth;

		// clear buffer for new mesh
		buffer_obj.clearBufferForRewrite();

		// append new mesh to buffer
		buffer_obj.appendMesh(myMesh);
	}

	// compute face centers for displaying the face normals
	if (face_normals) getCenters(myMesh, zFaceData, face_centers);
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	// draw buttonGroup
	B.draw();

	// draw mesh from the buffer
	drawLinesFromBuffer(buffer_obj, false);
	drawQuadsFromBuffer(buffer_obj, true);

	// draw face normals
	if (face_normals) drawMesh_FaceNormals(myMesh, face_centers, 2.0);
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