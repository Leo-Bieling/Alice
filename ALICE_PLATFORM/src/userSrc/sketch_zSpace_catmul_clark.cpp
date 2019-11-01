//#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v003
// zSpace header
#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>

using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
/*Objects*/
zObjMesh operateMeshObj;

/*Function sets*/
zFnMesh fnOperateMesh;

/*Tool sets*/

/*General variables*/
string path = "C:/Users/Leo.b/Desktop/test.obj";

/*Model - Display Setup*/
zModel model;

/*GUI Objects*/
double background = 0.2;
bool catmullClark = false;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(10000);
	fnOperateMesh = zFnMesh(operateMeshObj);
	fnOperateMesh.from(path, zOBJ);

	model.addObject(operateMeshObj);

	B = *new ButtonGroup(Alice::vec(50, 450, 0));

	B.addButton(&catmullClark, "Catmull Clark");
	B.buttons[0].attachToVariable(&catmullClark);
}

void update(int value)
{
	if (catmullClark) fnOperateMesh.smoothMesh(1);
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