//#define _MAIN_

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
zObjGraph operateGraphObj;

/*Function sets*/
zFnMesh fnInputMesh;
zFnGraph fnGraphFromMesh;
zFnGraph fnOperateGraph;

/*Tool sets*/
zTsGraphPolyhedra myGraphPolyhedra;

/*General variables*/
string meshPath = "C:/Users/Leo.b/Desktop/graph/dnipro_testbed/v3.json";
//string meshPath = "C:/Users/Leo.b/Desktop/graph/dnipro_testbed/test_1.json";
//string meshPath = "C:/Users/Leo.b/Desktop/graph/dnipro_testbed/placeholder_1.json";
string simplePath = "C:/Users/Leo.b/Desktop/graph/val5.txt";


zUtilsDisplay display;
zUtilsCore core;
zModel model;


bool drawGraph = true;
bool drawConHull = false;
bool drawGraphMesh = false;
bool drawDual = true;

double background = 0.2;

int snap = 0;
bool increaseSnap;
bool decreaseSnap;
bool drawDualMeshFaces = true;

int graphV_id = 0;

zPointArray p1;
zPointArray p2;

zItGraphHalfEdge heTmp;
zObjMesh tmpObj;

zObjMesh conHull;


////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	fnInputMesh = zFnMesh(inputMeshObj);
	fnInputMesh.from(meshPath, zJSON);

	zFnMesh tmpFn(tmpObj);

	tmpFn.from("C:/Users/Leo.b/Desktop/testVol.json", zJSON);



	//model.addObject(tmpObj);

	//zPointArray conHullPts;
	//tmpFn.getVertexPositions(conHullPts);

	//zFnMesh conHullFn(conHull);
	//conHullFn.makeConvexHull(conHullPts);
	//conHullFn.computeMeshNormals();

	//model.addObject(conHull);

	//zDoubleArray hedralAngles;
	//conHullFn.getEdgeDihedralAngles(hedralAngles);

	//vector<std::pair<double, int>> tmpPair;
	//for (int i = 0; i < hedralAngles.size(); i++)
	//{
	//	tmpPair.push_back(make_pair(hedralAngles[i], i));
	//}

	//std::sort(tmpPair.begin(), tmpPair.end());

	//zItMeshEdge e(conHull, tmpPair[0].second);
	//e.setColor(zColor(1, 0, 0, 1));
	//conHullFn.deleteEdge(e);


	fnGraphFromMesh = zFnGraph(graphFromMeshObj);
	//fnGraphFromMesh.createFromMesh(inputMeshObj, true, false);

	//graphFromMeshObj.setShowElements(true, true);
	//model.addObject(graphFromMeshObj);
	
	fnGraphFromMesh.from(simplePath, zMAYATXT);

	myGraphPolyhedra = zTsGraphPolyhedra(graphFromMeshObj);
	//myGraphPolyhedra.createGraphFromMesh(inputMeshObj);
	myGraphPolyhedra.snap = snap;
	myGraphPolyhedra.create();

	heTmp = zItGraphHalfEdge(graphFromMeshObj, graphV_id);

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

	//for (auto p : p1) model.displayUtils.drawPoint(p, zColor(1, 1, 0, 1), 4);
	//for (auto p : p2) model.displayUtils.drawPoint(p, zColor(0, 1, 1, 1), 4);

	//conHull.setShowFaces(true);
	//conHull.setShowEdges(true);
	//model.draw();
	


	//for (auto p : myGraphPolyhedra.tmp1) model.displayUtils.drawPoint(p, zColor(1, 1, 0, 1), 2);


	if (drawGraph) myGraphPolyhedra.drawGraph(true, 84);
	if (drawConHull) myGraphPolyhedra.drawConvexHulls();
	if (drawGraphMesh) myGraphPolyhedra.drawGraphMeshes();
	if (drawDual)
	{
		myGraphPolyhedra.drawDual(drawDualMeshFaces, true);
		/*for (int i = 0; i < myGraphPolyhedra.tmp1.size(); i++)
			display.drawLine(myGraphPolyhedra.tmp1[i], myGraphPolyhedra.tmp2[i]);*/
	
	}

}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{
	//if (k == '1')
	//{
	//	heTmp = heTmp.getPrev();

	//	printf("\n %i ", heTmp.getId());
	//	//graphV_id++;
	//}

}

void mousePress(int b, int state, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif // _MAIN_