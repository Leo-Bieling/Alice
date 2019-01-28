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

zMesh myMesh;
zGraph myGraph;

bool walk = false;
bool resetWalk = false;
int walkFrame = 0;

vector<int> sourceVertices = { 0,15 };
vector<double> vertexDistances;

vector<zVector> walkedEdges;
vector<zVector> currentWalkEdges;

double maxWalkingDistance = 10.0;
double currentWalkingDistance = 0.0;
double walkingStep = 0.1;

////// --- GUI OBJECTS ----------------------------------------------------
double background = 0.9;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------

void setup()
{
	fromOBJ(myMesh, "data/zSpace_walk_animation_mesh.obj"); // imports mesh from obj
	fromMESH(myGraph, myMesh); // creates graph from mesh
	walk_DistanceFromSources(myGraph, sourceVertices, vertexDistances); // compute vertex distances from source
	
	S = *new SliderGroup();
	S.addSlider(&background, "background");
	S.addSlider(&maxWalkingDistance, "maxDistance");
	S.sliders[1].attachToVariable(&maxWalkingDistance, 0, 20);

	B = *new ButtonGroup(vec(50, 450, 0));
	B.addButton(&walk, "walk");
	B.addButton(&resetWalk, "resetWalk");
}

void update(int value)
{
	if (walk)
	{
		walk_Animate(myGraph, currentWalkingDistance, vertexDistances, walkedEdges, currentWalkEdges);
	
		if (currentWalkingDistance < maxWalkingDistance && walkedEdges.size() < myGraph.numEdges()) currentWalkingDistance += walkingStep;
		else
		{
			walk = false;
		}

		walkFrame++;
	}

	if (resetWalk)
	{
		walkedEdges.clear();
		currentWalkEdges.clear();

		currentWalkingDistance = 0;

		resetWalk = !resetWalk;
	}


}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------

void draw()
{
	backGround(background);

	glColor3f(1, 0, 0);
	
	S.draw();
	B.draw();

	drawGraph(myGraph, false, true); // draw the graph
	
	// draw source vertices
	zColor sourceCol(0, 1, 0, 1);
	for (int i = 0; i < sourceVertices.size(); i ++) drawPoint(myGraph.vertexPositions[sourceVertices[i]], sourceCol, 15);
	
	// draw walked edges
	zColor walkCol(1, 0, 0, 1);
	for (int i = 0; i < walkedEdges.size(); i += 2) drawLine(walkedEdges[i], walkedEdges[i + 1], walkCol, 3);
	for (int i = 0; i < currentWalkEdges.size(); i += 2)
	{
		drawLine(currentWalkEdges[i], currentWalkEdges[i + 1], walkCol, 3);
		drawCircle(currentWalkEdges[i + 1], 16, 0.1, true, walkCol, 3);
	}
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