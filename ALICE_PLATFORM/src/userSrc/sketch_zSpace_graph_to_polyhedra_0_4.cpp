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
//#include <headers/zApp/include/zTsStatics.h>
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
//string path = "C:/Users/Leo.b/Desktop/graph/multi_node_graph.txt";
//string path = "C:/Users/Leo.b/Desktop/graph/simple_node_graph.txt";
string path = "C:/Users/Leo.b/Desktop/graph/complex.txt";

zModel model;

bool drawGraph = true;
bool drawConHull = false;
bool drawGraphMesh = true;

double background = 0.2;


////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(10000);

	// load graph
	fnOperateGraph = zFnGraph(operateGraphObj);
	fnOperateGraph.from(path, zMAYATXT);

	myGraphPolyhedra = zTsGraphPolyhedra(operateGraphObj);
	
	myGraphPolyhedra.createGraphMesh();

	//for (int i = 0; i < myGraphPolyhedra.colOfConHulls.size(); i++)
	//	model.addObject(myGraphPolyhedra.colOfConHulls[i]);
	
	model.addObject(*myGraphPolyhedra.graphObj);
	


	// buttons & sliders
	B = *new ButtonGroup(Alice::vec(50, 450, 0));
	B.addButton(&drawGraph, "drawGraph");
	B.buttons[0].attachToVariable(&drawGraph);
	B.addButton(&drawConHull, "drawConvexHull");
	B.buttons[1].attachToVariable(&drawConHull);
	B.addButton(&drawGraphMesh, "drawGraphMesh");
	B.buttons[2].attachToVariable(&drawGraphMesh);

}

void update(int value)
{
	operateGraphObj.setShowObject(drawGraph);
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	drawGrid(10);
	backGround(background);
	
	glPointSize(6.0);

	if (drawConHull) myGraphPolyhedra.drawConvexHulls();
	if (drawGraphMesh) myGraphPolyhedra.drawGraphMeshes();

	myGraphPolyhedra.drawDual();

	glColor3f(0, 0, 1);
	for (zItGraphEdge e(operateGraphObj); !e.end(); e++)
	{
		model.displayUtils.drawTextAtPoint(to_string(e.getId()), e.getCenter());
	}

	glColor3f(0, 1, 0);
	for (zItGraphVertex v(operateGraphObj); !v.end(); v++)
	{
		model.displayUtils.drawTextAtPoint(to_string(v.getId()), v.getPosition());
	}

	/*for (auto p : myGraphPolyhedra.tmpP)
		model.displayUtils.drawPoint(p);*/

	model.draw();

	glPointSize(1.0);
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