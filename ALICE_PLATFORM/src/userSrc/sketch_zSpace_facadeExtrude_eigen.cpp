
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
zObjMesh offsetMeshObj;
zObjMesh thicknessMeshObj;

zObjMesh operateMeshObj;
zObjBuffer myBufferObj;

/*Function sets*/
zFnMesh fnOperateMesh;
zFnMesh fnOffsetMesh;
zFnMesh fnThicknessMesh;



/*Tool sets*/
zTsVariableExtrude VE_operateMesh;
zTsVariableExtrude VE_offsetMesh;

zTsSpectral mySpectral;



string path = "data/hs_facade_healed.json";
string exportResultPath = "C:/Users/Leo.b/Desktop/spectral_result_export.json";
string exportMapPath = "C:/Users/Leo.b/Desktop/spectral_map_export.json";

vector<int> edgeIds;

double numEigenVectors = 30;
double eigenFrequency = 1;

bool offsetExtrude = false;
bool thicknessExtrude = false;

bool keepFacesTogether = true;
double minFaceOffset = 0.9;
double maxFaceOffset = 0.1;

double minFaceThickness = 0.4;
double maxFaceThickness = 2.0;

double faceWidthThreshold = 0.5;

////// --- GUI OBJECTS ----------------------------------------------------
bool computeEigenFunction = false;
bool computeEigenVectors = true;
bool reset = false;
bool exportResult = false;
bool exportMap = false;

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// create model buffer
	model = zModel(1000000);
	model.setShowBufQuads(true, true);
	model.setShowBufLines(true, false);
	model.setShowBufPoints(true, true);
	
	// import json
	//fnOperateMesh.computeFaceColorfromVertexColor();

	fnOperateMesh = zFnMesh(operateMeshObj);
	fnOffsetMesh = zFnMesh(offsetMeshObj);
	fnThicknessMesh = zFnMesh(thicknessMeshObj);

	// add mesh to model for display
	model.addObject(offsetMeshObj);
	model.addObject(thicknessMeshObj);
	model.addObject(operateMeshObj);

	// initialize and create mySpectral 
	mySpectral = zTsSpectral(operateMeshObj);
	mySpectral.createMeshfromFile(path, zJSON);
	operateMeshObj.appendToBuffer();
	
	//change color domain
	zDomainColor colDomain(zColor(0, 0, 0, 1), zColor(1, 0, 0, 1));
	mySpectral.setColorDomain(colDomain);

	////// --- GUI  ----------------------------------------------------

	// initialise slider group
	S = *new SliderGroup(vec(50, 50, 0));

	// addign and label sliders
	S.addSlider(&numEigenVectors, "numEigenVectors");
	S.sliders[0].attachToVariable(&numEigenVectors, 1, 200);
	S.addSlider(&eigenFrequency, "eigenFrequency");
	S.sliders[1].attachToVariable(&eigenFrequency, 0, 1000);
	S.addSlider(&minFaceOffset, "min offset");
	S.sliders[2].attachToVariable(&minFaceOffset, 0.0, 1.0);
	S.addSlider(&maxFaceOffset, "max offset");
	S.sliders[3].attachToVariable(&maxFaceOffset, 0.0, 1.0);
	S.addSlider(&minFaceThickness, "min thickness");
	S.sliders[4].attachToVariable(&minFaceThickness, 0.0, 10.0);
	S.addSlider(&maxFaceThickness, "max thickness");
	S.sliders[5].attachToVariable(&maxFaceThickness, 0.0, 10.0);

	// initialise button group
	B = *new ButtonGroup(vec(50, 500, 0));

	// assign and label buttons
	B.addButton(&computeEigenFunction, "compute eigenFunc");
	B.addButton(&computeEigenVectors, "compute eigenVec");
	B.addButton(&offsetExtrude, "offset extrude");
	B.addButton(&thicknessExtrude, "thickness extrude");
	B.addButton(&reset, "reset");
	B.addButton(&exportResult, "export result");
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

		cout << colors[0].r << " " << colors[0].g << " " << colors[0].b << endl;
		cout << fnOperateMesh.getVertexColor(0).b << endl;
	}
	
	if (offsetExtrude)
	{
		VE_operateMesh = zTsVariableExtrude(operateMeshObj);
		offsetMeshObj = VE_operateMesh.getVariableFaceOffset(keepFacesTogether, true, minFaceOffset, maxFaceOffset, false);

		fnOffsetMesh.setFaceColor(zColor(0.5, 0.5, 0.5, 1.0));

		offsetMeshObj.setShowObject(true);
		operateMeshObj.setShowObject(false);
		thicknessMeshObj.setShowObject(false);

		offsetExtrude = !offsetExtrude;
	}

	if (thicknessExtrude)
	{
		VE_offsetMesh = zTsVariableExtrude(offsetMeshObj);
		thicknessMeshObj = VE_offsetMesh.getVariableFaceThicknessExtrude(true, minFaceThickness, maxFaceThickness);
		
		fnThicknessMesh.setFaceColor(zColor(0.5, 0.5, 0.5, 1.0));
		
		thicknessMeshObj.setShowObject(true);
		operateMeshObj.setShowObject(false);
		offsetMeshObj.setShowObject(false);

		thicknessExtrude = !thicknessExtrude;
	}

	if (reset)
	{
		computeEigenFunction = false;
		eigenFrequency = 1;

		// toggle display for reset
		thicknessMeshObj.setShowObject(false);
		operateMeshObj.setShowObject(true);
		offsetMeshObj.setShowObject(false);

		// clear meshes for reset
		fnOffsetMesh.clear();
		fnThicknessMesh.clear();

		reset = !reset;
	}

	if (exportResult)
	{
		fnThicknessMesh.to(exportResultPath, zJSON);
		exportResult = !exportResult;
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
	//drawGrid(20.0);

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