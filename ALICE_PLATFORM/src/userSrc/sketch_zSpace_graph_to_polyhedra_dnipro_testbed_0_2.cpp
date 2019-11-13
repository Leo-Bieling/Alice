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

string meshPath = "C:/Users/Leo.b/Desktop/graph/dnipro_testbed/v3.json";
string simplePath = "C:/Users/Leo.b/Desktop/graph/val5.txt";


bool drawGraph = true;
bool drawConHull = false;
bool drawGraphMesh = false;
bool drawDual = true;
bool drawDualMeshFaces = false;

double background = 0.2;

int snap = 0;
bool increaseSnap;
bool decreaseSnap;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	//fnInputMesh = zFnMesh(inputMeshObj);
	//fnInputMesh.from(meshPath, zJSON);

	fnGraphFromMesh = zFnGraph(graphFromMeshObj);
	fnGraphFromMesh.from(simplePath, zMAYATXT);

	
	myGraphPolyhedra = zTsGraphPolyhedra(graphFromMeshObj); // NOTE: add the model as input and use it in the draw function
	//myGraphPolyhedra.createGraphFromMesh(inputMeshObj);
	//myGraphPolyhedra.snap = snap;
	myGraphPolyhedra.create();


	// buttons & sliders
	B = *new ButtonGroup(Alice::vec(50, 450, 0));
	B.addButton(&drawGraph, "drawGraph");
	B.buttons[0].attachToVariable(&drawGraph);
	B.addButton(&drawConHull, "drawConvexHull");
	B.buttons[1].attachToVariable(&drawConHull);
	B.addButton(&drawGraphMesh, "drawGraphMesh");
	B.buttons[2].attachToVariable(&drawGraphMesh);
	B.addButton(&drawDual, "drawDual");
	B.buttons[3].attachToVariable(&drawDual);
	B.addButton(&increaseSnap, "increaseSnap");
	B.buttons[4].attachToVariable(&increaseSnap);
	B.addButton(&decreaseSnap, "decreaseSnap");
	B.buttons[5].attachToVariable(&decreaseSnap);
	B.addButton(&drawDualMeshFaces, "drawDualMeshFaces");
	B.buttons[6].attachToVariable(&drawDualMeshFaces);
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
	drawGrid(10);
	backGround(background);


	myGraphPolyhedra.drawGraph(drawGraph, true);
	if (drawConHull) myGraphPolyhedra.drawConvexHulls();
	if (drawGraphMesh) myGraphPolyhedra.drawGraphMeshes();
	if (drawDual)
	{
		myGraphPolyhedra.drawDual(drawDualMeshFaces, true); // NOTE: add the connection line as a boolean here
		/*for (int i = 0; i < myGraphPolyhedra.tmp1.size(); i++)
			display.drawLine(myGraphPolyhedra.tmp1[i], myGraphPolyhedra.tmp2[i]);*/	
	}

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