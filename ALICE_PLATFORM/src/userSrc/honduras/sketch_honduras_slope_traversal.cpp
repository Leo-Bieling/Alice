#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zData.h>
#include <headers/include/zIO.h>

using namespace zSpace;
using namespace std;

zGraph rainflow(zMesh _mesh, int _id, vector<zVector> _faceCenters, int _steps)
{
	vector<int> tmpConnectedFacesIds;
	vector<double> tmpConnectedFacesZs;
	vector<zVector> graphPositions;
	vector<int> graphEdgeConnections;
	int lowestId;
	int tmpFaceId = _id;

	for (int i = 0; i < _steps; i++)
	{
		// get connected face Ids
		_mesh.getConnectedFaces(tmpFaceId, zFaceData, tmpConnectedFacesIds);

		// fill containers for graph
		graphPositions.push_back(_faceCenters[tmpFaceId]);
		graphEdgeConnections.push_back(i);

		// store z value of connected face centers in container
		for (int i = 0; i < tmpConnectedFacesIds.size(); i++)
			tmpConnectedFacesZs.push_back(_faceCenters[tmpConnectedFacesIds[i]].z);

		// sort z values in container
		sort(tmpConnectedFacesZs.begin(), tmpConnectedFacesZs.end());

		// assign first/ min value's Id to lowestId integer
		for (int i = 0; i < tmpConnectedFacesIds.size(); i++)
			if (tmpConnectedFacesZs[0] == _faceCenters[tmpConnectedFacesIds[i]].z) lowestId = tmpConnectedFacesIds[i];

		// assign lowestID to tmpFaceId for next loop
		tmpFaceId = lowestId;

		// clear container for next loop
		tmpConnectedFacesIds.clear();
		tmpConnectedFacesZs.clear();
	}

	return zGraph(graphPositions, graphEdgeConnections);
}

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
zMesh terrain;
zBufferObject bufferObj;
vector<zVector> faceCenters;
vector<zVector> seeds;

//double rotation;
int numSteps = 20;
//double distSteps;

int testFaceId1 = 10100;
int testFaceId2 = 14100;

////// --- GUI OBJECTS ----------------------------------------------------
bool smooth = false;
bool faceNormals = false;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// enable smooth display
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);

	// import mesh
	fromOBJ(terrain, "data/turtlingBayPatch.obj");
	getCenters(terrain, zFaceData, faceCenters);
	setVertexColor(terrain, zColor(0.5, 0.5, 0.5, 1), true);
	
	// create buffer
	bufferObj = zBufferObject(1000000);
	bufferObj.appendMesh(terrain);

	// get seeds
	seeds.push_back(faceCenters[testFaceId1]);
	seeds.push_back(faceCenters[testFaceId2]);

	// initialize buttonGroup
	B = *new ButtonGroup();
	B.addButton(&smooth, "smooth");
	B.addButton(&faceNormals, "show face normals");
}

void update(int value)
{
	if (smooth)
	{
		// smooth mesh
		smoothMesh(terrain);
		setVertexColor(terrain, zColor(0.5, 0.5, 0.5, 1), false);
		smooth = !smooth;

		// clear buffer and amend new mesh
		bufferObj.clearBufferForRewrite();
		bufferObj.appendMesh(terrain);
	}
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	// draw from buffer
	drawLinesFromBuffer(bufferObj);

	// draw face normals
	if (faceNormals) drawMesh_FaceNormals(terrain, faceCenters, 2.0);

	// draw seed points
	for (int i = 0; i < seeds.size(); i++)
		drawPoint(seeds[i], zColor(1, 0, (1 / 255 * 100), 1), 10);
	
	drawGraph(rainflow(terrain, testFaceId1, faceCenters, numSteps), true, true);
	drawGraph(rainflow(terrain, testFaceId2, faceCenters, numSteps), true, true);

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