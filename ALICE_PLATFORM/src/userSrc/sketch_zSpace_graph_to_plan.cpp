
//#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v002
// zSpace header
#include <headers/app/include/zObjects.h>
#include <headers/app/include/zFnSets.h>
#include <headers/app/include/zViewer.h>

// include toolset header
#include <headers/app/include/Tools/zTsGeometry.h>

using namespace zSpace;
using namespace std;


////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
/*Objects*/
zObjGraph inputGraphObj;
zObjMesh operateMeshObj;

/*Function sets*/
zFnGraph fnInputGraph;
zFnMesh fnOperateMesh;

/*Tool sets*/
string graph_path = "C:/Users/Leo.b/Desktop/graph_to_plan/graph.txt";
string mesh_path = "C:/Users/Leo.b/Desktop/graph_to_plan/PTP_mesh.json";

vector<int> in_edges;
vector<int> dual_edges;

////// --- MODEL / DISPLAY SETUP ----------------------------------------------------
zModel model;

////// --- GUI OBJECTS ----------------------------------------------------


////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(10000);
	//fnInputGraph = zFnGraph(inputGraphObj);
	//fnInputGraph.from(graph_path, zMAYATXT);

	fnOperateMesh = zFnMesh(operateMeshObj);
	fnOperateMesh.from(mesh_path, zJSON);

	fnOperateMesh.getDualGraph(inputGraphObj, in_edges, dual_edges, true);

	// append to model for displaying the object
	model.addObject(inputGraphObj);
	//model.addObject(operateMeshObj);

	////// --- GUI SETUP ----------------------------------------------------

}

void update(int value)
{

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	//inputGraphObj.draw();
	//inputGraphObj.drawGraph();
	//drawGrid(20);
	backGround(0.75);
	model.draw();
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