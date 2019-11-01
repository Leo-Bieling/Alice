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
//#include <headers/zApp/include/zTsGeometry.h>

using namespace zSpace;
using namespace std;


////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
/*Objects*/
zObjGraph operateGraphObj;



/*Function sets*/
zFnGraph fnOperateGraph;




/*Tool sets*/

/*General variables*/
string path = "C:/Users/Leo.b/Desktop/graph/graphTable.txt";



////// --- MODEL / DISPLAY SETUP ----------------------------------------------------
zModel model;
bool drawGraph = true;

////// --- GUI OBJECTS ----------------------------------------------------
double background = 0.2;




////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(10000);

	fnOperateGraph = zFnGraph(operateGraphObj);
	fnOperateGraph.from(path, zMAYATXT);
	model.addObject(operateGraphObj);
	operateGraphObj.setShowElements(true, true);

	cout << "\nnumEdges: " << fnOperateGraph.numEdges();
	cout << "\nnumHalfEdges: " << fnOperateGraph.numHalfEdges();
	cout << "\nnumEdges: " << fnOperateGraph.numVertices();


	for (zItGraphVertex v(operateGraphObj); !v.end(); v++)
	{
		if (!v.checkValency(1))
		{
			zItGraphHalfEdge tmp = v.getHalfEdge();
			fnOperateGraph.splitEdge(tmp.getId());
			printf("\nHalfeEdge %i got split", tmp.getId());
		}
	}

	cout << "\n-------AFTER--------";
	cout << "\nnumEdges: " << fnOperateGraph.numEdges();
	cout << "\nnumHalfEdges: " << fnOperateGraph.numHalfEdges();
	cout << "\nnumEdges: " << fnOperateGraph.numVertices();

	B = *new ButtonGroup(Alice::vec(50, 450, 0));

	B.addButton(&drawGraph, "drawGraph");
	B.buttons[0].attachToVariable(&drawGraph);
}

void update(int value)
{

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	drawGrid(10);
	backGround(background);

	operateGraphObj.setShowObject(drawGraph);

	glPointSize(4.0);
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