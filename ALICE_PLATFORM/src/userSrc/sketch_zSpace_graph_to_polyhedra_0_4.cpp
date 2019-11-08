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
zObjGraph operateGraphObj;

/*Function sets*/
zFnGraph fnOperateGraph;

/*Tool sets*/
zTsGraphPolyhedra myGraphPolyhedra;

/*General variables*/
string path = "C:/Users/Leo.b/Desktop/graph/baseline_tester.txt";
//string path = "C:/Users/Leo.b/Desktop/graph/newTest_5.txt";
//string path = "C:/Users/Leo.b/Desktop/graph/test.json"; // fix JSON

zUtilsDisplay display;

bool drawGraph = true;
bool drawConHull = false;
bool drawGraphMesh = false;
bool drawDual = true;

double background = 0.2;

int snap = 0;
bool increaseSnap;
bool decreaseSnap;
bool drawDualMeshFaces = true;



////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// load graph
	fnOperateGraph = zFnGraph(operateGraphObj);
	//fnOperateGraph.from(path, zJSON);
	fnOperateGraph.from(path, zMAYATXT);

	myGraphPolyhedra = zTsGraphPolyhedra(operateGraphObj);
	myGraphPolyhedra.snap = snap;
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
	
	if (drawGraph) myGraphPolyhedra.drawGraph(true);
	if (drawConHull) myGraphPolyhedra.drawConvexHulls();
	if (drawGraphMesh) myGraphPolyhedra.drawGraphMeshes();
	if (drawDual)
	{
		myGraphPolyhedra.drawDual(drawDualMeshFaces, true);
		for (int i = 0; i < myGraphPolyhedra.tmp1.size(); i++)
			display.drawLine(myGraphPolyhedra.tmp1[i], myGraphPolyhedra.tmp2[i]);
	
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