
//#define _MAIN_

#ifdef _MAIN_

// alice library
#include "main.h"

// zSpace v002
// zSpace library
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

/*!<model*/
zModel model;

/*!<Objects*/
vector<zObjMesh> forceObjs;
zObjGraph formObj;
vector <zObjMesh> polytopalObjs;

/*!<Function sets*/

/*!<Tool sets*/
zTsPolytopal myPolytopal;


double offset = 0.05;
double angleTolerance = 5.0;
double dT = 1;
double minmax_Edge = 0.3;
zIntergrationType intType = zRK4;

string dirPath = "C:/Users/leo.b/desktop/polyhedra";
vector<string> forceFiles;

string exportPath = "C:/Users/leo.b/desktop/polytopals/";

int subDIVS = 0;

bool createFormDiagram = true;
bool createForceDiagram = true;

bool computeTargets = true;

bool computeEquilibrium = false;

bool createPolytopal = false;
bool closePolytopal = false;

bool drawForceDiagram = false;
bool drawFormDiagram = true;
bool drawPolytopal = true;

bool exportPolytopal = false;
bool exportFormGraph = false;

////// --- GUI OBJECTS ----------------------------------------------------

double background = 0.9;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------

////// ---------------------------------------------------- MODEL  ----------------------------------------------------

void setup()
{
	model = zModel(100000);
	
	model.coreUtils.getFilesFromDirectory(forceFiles, dirPath, zJSON);
	printf("\n NF : %i ", forceFiles.size());
	
	forceObjs.assign(forceFiles.size(), zObjMesh());
	polytopalObjs.assign(forceFiles.size(), zObjMesh());	
	
	myPolytopal = zTsPolytopal(formObj, forceObjs, polytopalObjs);	

	model.addObject(formObj);
	


	for (int i = 0; i < forceObjs.size(); i++)
	{
		model.addObject(forceObjs[i]);
		model.addObject(polytopalObjs[i]);
	}

	// set display element booleans
	formObj.setShowElements(true, true);

	for (int i = 0; i < forceObjs.size(); i++)
	{
		forceObjs[i].setShowElements(false, true, false);
		polytopalObjs[i].setShowElements(false, true, true);
	}
	
	//////////////////////////////////////////////////////////
	S = *new SliderGroup();
	
	S.addSlider(&background, "background");

	S.addSlider(&offset, "offset");
	S.sliders[1].attachToVariable(&offset, 0.01, 1.00);
	
	S.addSlider(&angleTolerance, "angleTolerance");
	S.sliders[2].attachToVariable(&angleTolerance, 0.001, 40.00);
	
	S.addSlider(&minmax_Edge, "minmax_Edge");
	S.sliders[3].attachToVariable(&minmax_Edge, 0.001, 0.5);


	B = *new ButtonGroup(vec(50, 450, 0));

	B.addButton(&createForceDiagram, "createForceDiagram");
	B.addButton(&createFormDiagram, "createFormDiagram");	
	B.addButton(&computeEquilibrium, "computeEquilibrium");
	B.addButton(&createPolytopal, "createPolytopal");
	B.addButton(&closePolytopal, "closePolytopal");
	B.addButton(&drawForceDiagram, "drawForceDiagram");
	B.addButton(&drawFormDiagram, "drawFormDiagram");	
	B.addButton(&drawPolytopal, "drawPolytopal");
	B.addButton(&exportPolytopal, "exportPolytopals");
	B.addButton(&exportFormGraph, "exportFormGraph");

	//////////////////////////////////////////////////////////////////////////
}

void update(int value)
{
	formObj.setShowObject(drawFormDiagram);
	
	for (int i = 0; i < forceObjs.size(); i++)
	{
		forceObjs[i].setShowObject(drawForceDiagram);
		polytopalObjs[i].setShowObject(drawPolytopal);
	}

	myPolytopal.setVertexOffset(offset);
	
	if (createForceDiagram)
	{
		myPolytopal.createForceFromFiles(forceFiles, zJSON);

		for (int i = 0; i < myPolytopal.fnForces.size(); i++)
		{
			//printf("\n  %i numP: %i ",i, myPolytopal.fnForces[i].numPolygons());

			vector<zVector> fCenters;
			myPolytopal.fnForces[i].getCenters(zFaceData, fCenters);

			forceObjs[i].setShowFaceNormals(true, fCenters, 0.1);
		}

		createForceDiagram = !createForceDiagram;
	}

	
	if (createFormDiagram)
	{
		myPolytopal.createFormFromForce(offset, 1);

		computeTargets = true;

		printf("\n numE: %i ", myPolytopal.fnForm.numEdges());
		
		createFormDiagram = !createFormDiagram;
	}
	
	if (computeEquilibrium)
	{
		bool out = myPolytopal.equilibrium(computeTargets, minmax_Edge, dT, intType, 1, angleTolerance, true, true);;

		if (out) myPolytopal.setFormEdgeWeightsfromForce(zDomainDouble(2.0, 10.0));

		computeEquilibrium = !out;
	}

	if (createPolytopal)
	{		
		myPolytopal.createPolytopalsFromForce(subDIVS);

		if (closePolytopal) myPolytopal.closePolytopals();

		createPolytopal = !createPolytopal;
	}
	
	if (exportPolytopal)
	{
		for (int i = 0; i < myPolytopal.fnPolytopals.size(); i++)
		{
			string path1 = exportPath + "/polytopals_" + to_string(i) + ".json";

			myPolytopal.fnPolytopals[i].to(path1, zJSON);
		}

		exportPolytopal = !exportPolytopal;
	}	

	if (exportFormGraph)
	{
		string path2 = exportPath + "/formGraph.json";
		myPolytopal.fnForm.to(path2, zJSON);
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
}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------

void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'q') if (subDIVS < 3) subDIVS++;
	if (k == 'a') if (subDIVS >= 1) subDIVS--;
}

void mousePress(int b, int state, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif // _MAIN_