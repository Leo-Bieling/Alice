//#define _MAIN_


#ifdef _MAIN_

// alice library
#include "main.h"

// zSpace v002
// zSpace Library
#include <headers/app/include/zObjects.h>
#include <headers/app/include/zFnSets.h>
#include <headers/app/include/zViewer.h>

// zSpace toolset header
#include <headers/app/include/Tools/zTsStatics.h>

using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// CUSTOM METHODS ----------------------------------------------------

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------

zModel model;

zObjMesh forceObj;
zObjMesh formObj;
zObjMesh resultObj;

zTsMeshVault myVault;

double vLoad = 0.1;
double forceDensity = 1.0;

string path = "C:/Users/leo.b/Desktop/FDM.obj";
string exportPath = "C:/Users/leo.b/desktop/FDM_out.obj";

bool formFind = false;
bool setQ = false;
bool setLoad = false;
bool createDiagrams = false;
bool drawForceDiagram = false;
bool drawFormDiagram = false;
bool drawResult = true;

bool exportResult = false;

vector<zVector> forces;

////// --- GUI OBJECTS ----------------------------------------------------

double background = 0.9;
////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------


////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(100000);
	myVault = zTsMeshVault(resultObj, formObj, forceObj);
	
	//input mesh to be form found attached to the result obj
	myVault.createResultfromFile(path, zOBJ);

	// default set constraints - boundary vertices
	myVault.setConstraints(zResultDiagram);
	   
	// default set loads
	myVault.setVertexMass(vLoad);

	// default set forcedensities
	myVault.setForceDensity(forceDensity);
	
	// append to model for displaying the object
	resultObj.setShowElements(false, true, true);
	model.addObject(resultObj);

	model.addObject(formObj);	
	model.addObject(forceObj);
		
	//////////////////////////////////////////////////////////
	S = *new SliderGroup();
	
	S.addSlider(&background, "background");

	S.addSlider(&vLoad, "vLoad");
	S.sliders[1].attachToVariable(&vLoad, -10.0, 10.0);

	S.addSlider(&forceDensity, "forceDensity");
	S.sliders[2].attachToVariable(&forceDensity, 0.001, 10.0);

	/////////////////////////////
	B = *new ButtonGroup(vec(50, 450, 0));

	B.addButton(&setQ, "setForceDensity");
	B.addButton(&setLoad, "setLoad");
	B.addButton(&formFind, "formFind");
	B.addButton(&createDiagrams, "createDiagrams");
	B.addButton(&drawForceDiagram, "drawForceDiagram");
	B.addButton(&drawFormDiagram, "drawFormDiagram");
	B.addButton(&drawResult, "drawResult");		
	B.addButton(&exportResult, "e_result");

	//////////////////////////////////////////////////////////////////////////


}

void update(int value)
{
	formObj.setShowObject(drawFormDiagram);
	forceObj.setShowObject(drawForceDiagram);
	resultObj.setShowObject(drawResult);

	// set force densities
	if (setQ) myVault.setForceDensity(forceDensity);

	// set loads
	if (setLoad) myVault.setVertexMass(vLoad);

	if (formFind)
	{
		myVault.forceDensityMethod();

		myVault.getForces_GradientDescent(forces);
	}

	if (createDiagrams)
	{
		//create form from result
		myVault.createFormFromResult();

		//create force from form
		myVault.createForceFromForm(true);

		//set display elements 
		formObj.setShowElements(false, true, false);
		forceObj.setShowElements(false, true, false);

		createDiagrams = !createDiagrams;
	}

	if (exportResult)
	{
		myVault.fnResult.to(exportPath, zOBJ);
		exportResult = !exportResult;
	}
}


////// ---------------------------------------------------- VIEW  ----------------------------------------------------

void draw()
{
	backGround(background);
	//drawGrid(20.0);
	
	S.draw();
	B.draw();
	
	model.draw();

	zVector *pos = myVault.fnResult.getRawVertexPositions();
	for (int i = 0; i < myVault.fnResult.numVertices(); i++)
	{
		if (myVault.fnResult.onBoundary(i))
		{
			model.displayUtils.drawPoint(pos[i], zColor(1, 0, 0, 1), 3);
		}
	}

	//if (forces.size() > 0)
	//{
	//	zVector *pos = myVault.fnResult.getRawVertexPositions();
	//	for (int i = 0; i < forces.size(); i++)
	//	{
	//		zVector p1 = forces[i];
	//		//p1.normalize();
	//		p1 *= 1.5;

	//		model.displayUtils.drawLine(pos[i], pos[i] + p1, zColor(1,0,0,1), 3);
	//	}
	//}
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