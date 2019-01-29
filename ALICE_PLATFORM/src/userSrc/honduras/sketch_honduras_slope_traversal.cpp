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
vector<zVector> faceCenters;

vector<zVector> seeds;
vector<zVector> polyPts;
double rotation;
int numSteps;
double distSteps;

////// --- GUI OBJECTS ----------------------------------------------------
bool smooth = false;
bool faceNormals = false;

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
	//triangulate	(terrain);
	getCenters(terrain, zFaceData, faceCenters);
	

	// get seeds
	seeds.push_back(faceCenters[10000]);


	// put seeds as first point in polyPts
	polyPts.push_back(seeds[0]);


	// initialize buttonGroup
	B = *new ButtonGroup();
	B.addButton(&smooth, "smooth");
	B.addButton(&faceNormals, "show face normals");

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
	}


}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	// draw mesh 
	drawMesh(terrain, false, true, true);

	// draw face normals
	if (faceNormals) drawMesh_FaceNormals(terrain, faceCenters, 2.0);

	// draw seed points
	for (int i = 0; i < seeds.size(); i++)
	{
		drawPoint(seeds[i], zColor(1, 0, (1 / 255 * 100), 1), 50);
	}


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