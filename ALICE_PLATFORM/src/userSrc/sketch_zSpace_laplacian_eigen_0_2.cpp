
//#define _MAIN_

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
/*model*/
zModel model;

/*Objects*/
zObjMesh operateMeshObj;
zObjBuffer myBufferObj;

/*Function sets*/
zFnMesh fnOperateMesh;

/*Tool sets*/
zTsSpectral mySpectral;


string path = "data/hs_facade_healed.json";
string exportMapPath = "C:/Users/Leo.b/Desktop/spectral_map_export.json";

vector<int> edgeIds;

double numEigenVectors = 30;
double eigenFrequency = 1;

////// --- GUI OBJECTS ----------------------------------------------------
bool computeEigenFunction = false;
bool computeEigenVectors = true;
bool exportMap = false;

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// create model buffer
	model = zModel(1000000);
	model.setShowBufQuads(true, true);
	model.setShowBufLines(true, false);

	// assign fn
	fnOperateMesh = zFnMesh(operateMeshObj);


	// initialize and create mySpectral 
	mySpectral = zTsSpectral(operateMeshObj);
	mySpectral.createMeshfromFile(path, zJSON);
	
	//set color domain
	zDomainColor colDomain(zColor(0, 1, 1), zColor(180, 1, 1));
	mySpectral.setColorDomain(colDomain, zHSV);

	// add mesh to model for display
	model.addObject(operateMeshObj);
	operateMeshObj.appendToBuffer();

	////// --- GUI  ----------------------------------------------------

	// initialise slider group
	S = *new SliderGroup(vec(50, 50, 0));

	// addign and label sliders
	S.addSlider(&numEigenVectors, "numEigenVec");
	S.sliders[0].attachToVariable(&numEigenVectors, 1, 200);
	S.addSlider(&eigenFrequency, "eigenF");
	S.sliders[1].attachToVariable(&eigenFrequency, 0, 10000);

	// initialise button group
	B = *new ButtonGroup(vec(50, 500, 0));

	// assign and label buttons
	B.addButton(&computeEigenFunction, "compute eigenFunc");
	B.addButton(&computeEigenVectors, "compute eigenVec");
	B.addButton(&exportMap, "export map");
}

void update(int value)
{
	if (computeEigenFunction)
	{	
		mySpectral.setNumEigens(floor(numEigenVectors));

		double eValue = mySpectral.computeEigenFunction(eigenFrequency, computeEigenVectors);
		zColor* colors = fnOperateMesh.getRawVertexColors();
		int id = fnOperateMesh.getVBOVertexColorIndex();
		model.displayUtils.bufferObj.updateVertexColors(colors, fnOperateMesh.numVertices(), id);

		computeEigenVectors = false;
	}
	
	
	if (exportMap)
	{
		fnOperateMesh.to(exportMapPath, zJSON);
		exportMap = !exportMap;
	}
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);

	B.draw();
	S.draw();

	model.draw();

	setup2d();
	drawString("Press 'q' to increase the eigenFrequency", vec(50, 350, 0));
	drawString("Press 'a' to decrease the eigenFrequency", vec(50, 375, 0));
	drawString(("eigenFrequency: " + to_string((int)eigenFrequency)), vec(50, 400, 0));
	restore3d();
}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'q') eigenFrequency++;
	if (k == 'a')
	{
		eigenFrequency--;
		if (eigenFrequency < 0)	eigenFrequency = 0;
	}	
}

void mousePress(int b, int state, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif _MAIN_