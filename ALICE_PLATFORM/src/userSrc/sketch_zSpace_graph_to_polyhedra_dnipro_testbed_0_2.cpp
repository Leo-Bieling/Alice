#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v003
// zSpace header
#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>

// include toolset header
#include <headers/zToolsets/geometry/zTsGraphPolyhedra.h>

using namespace zSpace;
using namespace std;

/*Objects*/
zObjMesh inputMeshObj;
zObjGraph graphFromMeshObj;

/*Function sets*/
zFnMesh fnInputMesh;
zFnGraph fnGraphFromMesh;

/*Tool sets*/
zTsGraphPolyhedra myGraphPolyhedra;

/*Utilities*/
zUtilsDisplay display;
zModel model;

/*General variables*/

//string path = "C:/Users/Leo.b/Desktop/graph/valence_4.txt";
//string path = "C:/Users/Leo.b/Desktop/graph/valence_4_2.txt";
//string path = "C:/Users/Leo.b/Desktop/graph/valence_4_aggregation.txt";
string path = "C:/Users/Leo.b/Desktop/graph/valence_5.txt";
//string path = "C:/Users/Leo.b/Desktop/graph/valence_6.txt";

string meshPath = "C:/Users/Leo.b/Desktop/graph/dnipro.json";

double background = 1.0;

bool drawGraph = true;
bool drawGraphIds = false;
bool drawNodeIds = false;
bool drawCef = false;
bool drawCfc = false;

bool drawConHull = false;
bool drawConHullFaces = true;
bool drawConHullIds = false;

bool drawPrimalMesh = false;
bool drawPrimalMeshFaces = true;
bool drawPrimalMeshIds = false;

bool drawDualGraph = false;
bool drawDualGIds = false;

bool drawDualMesh = false;
bool drawDualMConnectivity = false;
bool drawDualMFaces = true;
bool drawDualMIds = false;
int snap = 0;
bool increaseSnap;
bool decreaseSnap;


////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	fnInputMesh = zFnMesh(inputMeshObj);
	fnInputMesh.from(meshPath, zJSON);
	
	fnGraphFromMesh = zFnGraph(graphFromMeshObj);
	fnGraphFromMesh.from(path, zMAYATXT);
	
	myGraphPolyhedra = zTsGraphPolyhedra(graphFromMeshObj); // FIX: add the model as input and use it in the draw function
	//myGraphPolyhedra.createGraphFromMesh(inputMeshObj, zVector(0, 0, -4));
	myGraphPolyhedra.snapSteps = snap;
	myGraphPolyhedra.create();

	// buttons & sliders
	#pragma region "buttons"
	B1 = *new ButtonGroup(Alice::vec(50, 50, 0));
	B1.addButton(&drawGraph, "Graph");
	B1.buttons[0].attachToVariable(&drawGraph);
	B1.addButton(&drawGraphIds, "GraphIds");
	B1.buttons[1].attachToVariable(&drawGraphIds);
	B1.addButton(&drawNodeIds, "NodeIds");
	B1.buttons[2].attachToVariable(&drawNodeIds);
	B1.addButton(&drawCef, "drawCef");
	B1.buttons[3].attachToVariable(&drawCef);
	B1.addButton(&drawCfc, "drawCfc");
	B1.buttons[4].attachToVariable(&drawCfc);

	B2 = *new ButtonGroup(Alice::vec(50, 300, 0));
	B2.addButton(&drawConHull, "ConvexHull");
	B2.buttons[0].attachToVariable(&drawConHull);
	B2.addButton(&drawConHullIds, "ConHullIds");
	B2.buttons[1].attachToVariable(&drawConHullIds);
	B2.addButton(&drawConHullFaces, "ConHullFaces");
	B2.buttons[2].attachToVariable(&drawConHullFaces);

	B3 = *new ButtonGroup(Alice::vec(50, 450, 0));
	B3.addButton(&drawPrimalMesh, "PrimalMesh");
	B3.buttons[0].attachToVariable(&drawPrimalMesh);
	B3.addButton(&drawPrimalMeshIds, "PrimalMeshIds");
	B3.buttons[1].attachToVariable(&drawPrimalMeshIds);
	B3.addButton(&drawPrimalMeshFaces, "PrimalMeshFaces");
	B3.buttons[2].attachToVariable(&drawPrimalMeshFaces);

	B4 = *new ButtonGroup(Alice::vec(50, 650, 0));
	B4.addButton(&drawDualGraph, "DualGraph");
	B4.buttons[0].attachToVariable(&drawDualGraph);
	B4.addButton(&drawDualGIds, "DualGIds");
	B4.buttons[1].attachToVariable(&drawDualGIds);

	B5 = *new ButtonGroup(Alice::vec(50, 800, 0));
	B5.addButton(&drawDualMesh, "DualMesh");
	B5.buttons[0].attachToVariable(&drawDualMesh);
	B5.addButton(&drawDualMIds, "DualMIds");
	B5.buttons[1].attachToVariable(&drawDualMIds);
	B5.addButton(&drawDualMFaces, "DualMFaces");
	B5.buttons[2].attachToVariable(&drawDualMFaces);

	B5.addButton(&drawDualMConnectivity, "DualMConnectivity");
	B5.buttons[3].attachToVariable(&drawDualMConnectivity);
	B5.addButton(&increaseSnap, "increaseSnap");
	B5.buttons[4].attachToVariable(&increaseSnap);
	B5.addButton(&decreaseSnap, "decreaseSnap");
	B5.buttons[5].attachToVariable(&decreaseSnap);
	#pragma endregion
}

void update(int value)
{
	if (increaseSnap)
	{
		snap++;
		setup();
		increaseSnap = !increaseSnap;
	}

	if (decreaseSnap)
	{
		snap--;
		setup();
		decreaseSnap = !decreaseSnap;
	}
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	//drawGrid(10);
	backGround(background);

	#pragma region"draw_methods"
	myGraphPolyhedra.drawGraph(drawGraph, drawGraphIds, drawGraphIds);
	myGraphPolyhedra.drawNodeIds(drawNodeIds);
	myGraphPolyhedra.drawFaceEdgeConnectivityIds(drawCef);
	myGraphPolyhedra.drawCellFaceConnectivityIds(drawCfc);
	myGraphPolyhedra.drawConvexHulls(drawConHull, drawConHullFaces, drawConHullIds, drawConHullIds, drawConHullIds);
	myGraphPolyhedra.drawPrimalMeshes(drawPrimalMesh, drawPrimalMeshFaces, drawPrimalMeshIds, drawPrimalMeshIds, drawPrimalMeshIds);
	myGraphPolyhedra.drawDualGraphs(drawDualGraph, drawDualGIds, drawDualGIds);
	myGraphPolyhedra.drawDualMeshes(drawDualMesh, drawDualMConnectivity, drawDualMFaces, drawDualMIds, drawDualMIds, drawDualMIds);
	#pragma endregion
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