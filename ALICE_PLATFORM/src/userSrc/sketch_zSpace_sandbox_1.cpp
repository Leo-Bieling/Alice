//#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v003
// zSpace header
#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>

// zSpace toolset header
#include <headers/zApp/include/zTsStatics.h>

using namespace zSpace;
using namespace std;

/*objects*/
zObjMesh operateMeshObj;
zObjGraph operateGraphObj;

/*function sets*/
zFnMesh fnOperateMesh;
zFnGraph fnOperateGraph;

/*tool sets*/

/*model setup*/
zModel model;

/*general variables*/
double background = 0.2;

string path = "C:/Users/Leo.b/Desktop/raw_mesh.json";

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(10000);
	fnOperateMesh = zFnMesh(operateMeshObj);
	fnOperateMesh.from(path, zJSON);

	fnOperateGraph = zFnGraph(operateGraphObj);
	fnOperateGraph.createFromMesh(operateMeshObj, true);

	operateGraphObj.setShowVertices(true);
	model.addObject(operateGraphObj);
}

void update(int value)
{
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	drawGrid(50);
	backGround(background);
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