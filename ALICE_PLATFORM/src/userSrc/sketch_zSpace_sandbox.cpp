
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
//#include <headers/zApp/include/zTsGeometry.h>

using namespace zSpace;
using namespace std;

/*objects*/
zObjGraph operateGraphObj;

/*function sets*/
zFnGraph fnOperateGraph;

/*tool sets*/

/*model setup*/
zModel model;

/*general variables*/
double background = 0.2;
bool drawGraph = true;

string path = "C:/Users/Leo.b/Desktop/graph/test.json";


////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(10000);
	fnOperateGraph = zFnGraph(operateGraphObj);
	fnOperateGraph.from(path, zJSON);
	model.addObject(operateGraphObj);

	zItGraphVertex v(operateGraphObj, 0);

	//zIntArray cEdges;
	//v.getConnectedEdges(cEdges);

	//for (auto e : cEdges) printf("\n %i ", e);



	/*gui setup*/
	B = *new ButtonGroup(Alice::vec(50, 450, 0));

	B.addButton(&drawGraph, "drawGraph");
	B.buttons[0].attachToVariable(&drawGraph);
}

void update(int value)
{
	operateGraphObj.setShowObject(drawGraph);
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	drawGrid(50);
	backGround(background);
	model.draw();

	glColor3f(0, 0, 1);
	for (zItGraphEdge e(operateGraphObj); !e.end(); e++)
		model.displayUtils.drawTextAtPoint(to_string(e.getId()), e.getCenter());
	
	glColor3f(0, 1, 0);
	for (zItGraphVertex v(operateGraphObj); !v.end(); v++)
		model.displayUtils.drawTextAtPoint(to_string(v.getId()), v.getPosition());

	
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