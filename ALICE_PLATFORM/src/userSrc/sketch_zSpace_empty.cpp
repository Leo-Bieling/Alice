
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
zObjMesh operateMeshObj;

/*Function sets*/
zFnMesh fnOperateMesh;

/*Tool sets*/
zTsRemesh myRemesh;


string path = "C:/Users/Leo.b/Desktop/test.obj";

////// --- MODEL / DISPLAY SETUP ----------------------------------------------------
zModel model;

////// --- GUI OBJECTS ----------------------------------------------------


////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(10000);
	fnOperateMesh = zFnMesh(operateMeshObj);
	fnOperateMesh.from(path, zOBJ);

	myRemesh = zTsRemesh(operateMeshObj);

	// append to model for displaying the object
	model.addObject(operateMeshObj);

	////// --- GUI SETUP ----------------------------------------------------

}

void update(int value)
{

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	//drawGrid(20);
	backGround(0.75);
	model.draw();
}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'q')
		myRemesh.tangentialRelaxation();
}

void mousePress(int b, int state, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif // _MAIN_