
#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v002
// zSpace header
#include <headers/app/include/zObjects.h>
#include <headers/app/include/zFnSets.h>
#include <headers/app/include/zViewer.h>

// zSpace toolset header
#include <headers/app/include/Tools/zTsGeometry.h>

using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
/*!<model*/
zModel model;

/*!<Objects*/
zObjMesh operateMesh;

/*!<Function sets*/
zFnMesh fnOperateMesh;

/*!<Tool sets*/
zTsSpectral mySpectral;

zObjBuffer myBuffer;

string path = "data/spectral_mesh_tester_scm.json";
//string path = "data/hs_comp_facade_family_rgb.json";

double frequency = 1;

////// --- GUI OBJECTS ----------------------------------------------------

bool computeEigenF = false;
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// create model buffer
	model = zModel(100000);

	// set up mySpectral and compute the laplacian
	mySpectral = zTsSpectral(operateMesh);
	mySpectral.createMeshfromFile(path, zJSON);
	mySpectral.computeMeshLaplcian(false);

	// changing color domain
	zDomainColor newColDomain(zColor(1, 0, 0, 1), zColor(0, 0, 1, 1));
	mySpectral.setColorDomain(newColDomain);

	// add mesh to model for display
	model.addObject(operateMesh);
	operateMesh.appendToBuffer();

	model.setShowBufQuads(true, true);
	model.setShowBufLines(true, false);

	////// --- GUI  ----------------------------------------------------
	//S = *new SliderGroup();
	//S.addSlider(&frequency, "frequency");
	//S.sliders[0].attachToVariable(&frequency, 0, 500);

	B = *new ButtonGroup(vec(50, 450, 0));
	B.addButton(&computeEigenF, "compute Eigenfunc");
}

void update(int value)
{
	if (computeEigenF)
	{
		mySpectral.computeEigenFunction(frequency);
		vector <zColor> colors;
		mySpectral.fnMesh.getVertexColors(colors);

		model.displayUtils.bufferObj.updateVertexColors(colors, operateMesh.mesh.VBO_VertexColorId);
	}



}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	//drawGrid(20.0);

	B.draw();
	S.draw();

	model.draw();

	

	setup2d();
	drawString("Press 'q' to increase the frequency", vec(50, 250, 0));
	drawString("Press 'a' to decrease the frequency", vec(50, 275, 0));
	drawString(("number of family types: " + to_string((int)frequency)), vec(50, 300, 0));
	restore3d();
}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'q') frequency++;
	if (k == 'a')
	{
		frequency--;
		if (frequency <= 1)	frequency = 1;
	}
}

void mousePress(int b, int state, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif _MAIN_