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

zGraph getRainflowGraph(zMesh _mesh, int _id, vector<zVector> _faceCenters, int _steps)
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
		if (i != 0)
		{
			graphEdgeConnections.push_back(i - 1);
			graphEdgeConnections.push_back(i);
		}

		// store z value of connected face centers in container
		for (int i = 0; i < tmpConnectedFacesIds.size(); i++)
			tmpConnectedFacesZs.push_back(_faceCenters[tmpConnectedFacesIds[i]].z);

		// sort z values in container
		sort(tmpConnectedFacesZs.begin(), tmpConnectedFacesZs.end());

		// assign first/ min value's Id to lowestId integer
		for (int i = 0; i < tmpConnectedFacesIds.size(); i++)
			if (tmpConnectedFacesZs[0] == _faceCenters[tmpConnectedFacesIds[i]].z && tmpConnectedFacesZs[0] < _faceCenters[tmpFaceId].z)
				lowestId = tmpConnectedFacesIds[i];

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
zMesh existBuildings; 
zMesh existRoads;
zMesh siteBoundary;
zBufferObject bufferObj;
vector<zVector> faceCenters;
vector<int> seeds;
vector<zGraph> rainflowGraphs;
vector<zVector> positions;

//double rotation;
int maxNumSteps = 50;
//double distSteps;

int testFaceId0 = 10750;
int testFaceId1 = 10900;
int testFaceId2 = 10800;
int testFaceId3 = 10850;
int testFaceId4 = 10700;
int testFaceId5 = 10820;


////// --- GUI OBJECTS ----------------------------------------------------
bool faceNormals = false;
bool showExistRoads = true;
bool showExistBuildings = true;
bool showSiteBoundary = false;

//triangular mesh
//once calculate the intial face
//line_PlaneIntersection()
//pointInTriangle()
// then get all neighboring faces and calculate it only with them when moving

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// enable smooth display
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);

	// import mesh
	//fromOBJ(terrain, "data/turtlingBayPatch_terrain.obj");
	fromOBJ(terrain, "data/turtlingBayPatch_terrain_tri.obj");
	getCenters(terrain, zFaceData, faceCenters);

	//import seed points
	fromTXT(positions, "data/turtlingBayPatch_seedPts.txt");

	// color water and land
	for (int i = 0; i < terrain.vertexColors.size(); i++)
	{
		if (terrain.vertexPositions[i].z < -8.629) terrain.vertexColors[i] = zColor(0.0, 0.0, 0.5, 1.0);
		else terrain.vertexColors[i] = zColor(0.5, 0.5, 0.5, 1.0);
	}

	// import roads and buildings
	fromOBJ(existRoads, "data/turtlingBayPatch_streets.obj");
	setVertexColor(existRoads, zColor(0.0, 0.0, 0.0, 1.0), true);
	fromOBJ(existBuildings, "data/turtlingBayPatch_buildings.obj");
	setVertexColor(existBuildings, zColor(0.5, 0.5, 0.5, 1.0), true);
	fromOBJ(siteBoundary, "data/turtlingBayPatch_siteBoundary.obj");
	setVertexColor(siteBoundary, zColor(1, 0.0, (1 / 255 * 100), 1.0), true);
	
	// create buffer
	bufferObj = zBufferObject(1000000);
	bufferObj.appendMesh(terrain);

	// get seeds
	seeds.push_back(testFaceId0);
	seeds.push_back(testFaceId1);
	seeds.push_back(testFaceId2);
	seeds.push_back(testFaceId3);
	seeds.push_back(testFaceId4);
	seeds.push_back(testFaceId5);

	// compute rainflow
	for (int i = 0; i < seeds.size(); i++)
		rainflowGraphs.push_back(getRainflowGraph(terrain, seeds[i], faceCenters, maxNumSteps));

	// initialize buttonGroup
	B = *new ButtonGroup(vec(75, 20, 0));
	B.addButton(&faceNormals, "show face normals");
	B.addButton(&showExistRoads, "show exist roads");
	B.addButton(&showExistBuildings, "show exist buildings");
	B.addButton(&showSiteBoundary, "show site boundary");

}

void update(int value)
{

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(1.0);
	//drawGrid(20.0);

	// draw from buffer
	drawLinesFromBuffer(bufferObj);

	// draw meshes
	if(showExistBuildings) 
		drawMesh(existBuildings, false, true, true);
	if(showExistRoads) 
		drawMesh(existRoads, false, false, true);
	if (showSiteBoundary)
		drawMesh(siteBoundary, false, true, true);

	// draw face normals
	if (faceNormals) 
		drawMesh_FaceNormals(terrain, faceCenters, 2.0);

	// draw seed points
	for (int i = 0; i < seeds.size(); i++)
		drawPoint(faceCenters[seeds[i]], zColor(0, 0.5, 0, 1), 10);

	// draw seed points
	for (int i = 0; i < positions.size(); i++)
		drawPoint(positions[i], zColor(1, 0, 0, 1), 10);
	
	// draw gaphs
	for (int i = 0; i < rainflowGraphs.size(); i++)
		drawGraph(rainflowGraphs[i], true, true);
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