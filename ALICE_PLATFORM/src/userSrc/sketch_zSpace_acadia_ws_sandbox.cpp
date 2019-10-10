
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
#include <headers/zApp/include/zTsGeometry.h>

using namespace zSpace;
using namespace std;


////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
/*Objects*/
zObjMesh meshObj;
zObjMeshArray trainingMeshObjs;

/*Function sets*/
zFnMesh fnMesh;
vector<zFnMesh> fnTrainingMeshes;
/*Tool sets*/


string path = "C:/Users/Leo.b/Desktop/open_cube.json";
int trainingSetSize = 25;
double range = 0.4;
////// --- MODEL / DISPLAY SETUP ----------------------------------------------------
zModel model;

////// --- GUI OBJECTS ----------------------------------------------------


////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(10000);
	fnMesh = zFnMesh(meshObj);

	fnMesh.from(path, zJSON);

	//zPointArray vertPos;
	//
	//fnMesh.getVertexPositions(vertPos);
	//
	//printf("\nrandom: %i", rand() % 10 + 5);
	//
	//for (int i = 0; i < vertPos.size(); i++)
	//{
	//	vertPos[i] += zVector((rand() % 1 + -1), (rand() % 1 + -1), (rand() % 1 + -1));
	//}

	//fnMesh.setVertexPositions(vertPos);

	//fnMesh.smoothMesh(1, false);

	// append to model for displaying the object
	model.addObject(meshObj);


	////// --- GUI SETUP ----------------------------------------------------

}

void update(int value)
{

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	drawGrid(20);
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