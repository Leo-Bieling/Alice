//#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zData.h>
#include <headers/include/zIO.h>

using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------

vector<int> edgePath;
zMesh myMesh;
zGraph myGraph;
vector<zVector> faceCenters;
bool faceNormals;

////// --- GUI OBJECTS ----------------------------------------------------



////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	fromOBJ(myMesh, "data/zSpace_walk_animation_mesh.obj");
	fromMESH(myGraph, myMesh);

	getCenters(myMesh, zFaceData, faceCenters);

	shortestPath(myGraph, 20, 62, zEdgePath, edgePath);
	printf("\n edgepath : ");
	for (int i = 0; i < edgePath.size(); i++)
	{
		printf(" %i ", edgePath[i]);

		myGraph.edgeColors[edgePath[i]] = zColor(1, 0, 0, 1);
		myGraph.edgeWeights[edgePath[i]] = 10;

		(edgePath[i] % 2 == 0) ? myGraph.edgeColors[edgePath[i] + 1] = zColor(1, 0, 0, 1) : myGraph.edgeColors[edgePath[i] - 1] = zColor(1, 0, 0, 1);
		(edgePath[i] % 2 == 0) ? myGraph.edgeWeights[edgePath[i] + 1] = 10 : myGraph.edgeWeights[edgePath[i] - 1] = 10;

	}

	B = *new ButtonGroup();
	B.addButton(&faceNormals, "faceNormals");
}

void update(int value)
{
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	drawGraph(myGraph);

	B.draw();

	if(faceNormals) drawMesh_FaceNormals(myMesh, faceCenters, 1.0);
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