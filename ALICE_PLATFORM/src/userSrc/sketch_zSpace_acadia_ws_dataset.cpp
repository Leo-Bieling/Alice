//#define _MAIN_

#ifdef _MAIN_

#include <chrono>

// alice header
#include "main.h"

// zSpace v003
// zSpace header
#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>

// include toolset header
#include <headers/zApp/include/zTsGeometry.h>

using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
/*Objects*/
zObjMesh meshObj;

/*Function sets*/
zFnMesh fnMesh;

/*Tool sets*/
zTsMesh2Pix mesh2Pix;

////// --- MODEL / DISPLAY SETUP ----------------------------------------------------
zModel model;

/*General variables*/
string meshPath = "C:/Users/Leo.b/Desktop/cube.json";
string imgOutputPath = "C:/Users/Leo.b/.spyder-py3/data_sets/dataSet_v13";

double angle = 45.00;
float translationRange = 0.4;

////// --- GUI OBJECTS ----------------------------------------------------
double background = 0.2;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(10000);

	fnMesh = zFnMesh(meshObj);
	fnMesh.from(meshPath, zJSON);

	mesh2Pix = zTsMesh2Pix(meshObj, 256);
	//mesh2Pix.generatePrintSupport2Pix(imgOutputPath, "mesh2Pix", angle, true, 500, true, zVector(translationRange, translationRange, 0));

	mesh2Pix.predictPrintSupport2Pix("C:/Users/Leo.b/Desktop/test/test/", "m2p_test_0", false);

	meshObj.setShowElements(true, true, false);
	model.addObject(meshObj);

}

void update(int value)
{
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	drawGrid(20);
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