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

zGraph getRainflowGraph(zMesh _mesh, zVector _seedPos, int _id, vector<zVector> _faceCenters, int _steps)
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

zGraph getSlopeTraversalGraph(zMesh _mesh, zVector _seedPos, vector<zVector> _faceCenters, double _rotation, double _stepDist, int _numOfSteps)
{
	int intersectionFaceId;
	zVector internalSeedPos = _seedPos;
	zVector intersectionPt;
	zVector newIntersetionPt;
	zVector direction;

	vector<zVector> graphPositions;
	vector<int> graphEdgeConnections;


	for (int i = 0; i < _numOfSteps; i++)
	{
		zVector seedPosTransUp = internalSeedPos + zVector(0, 0, 100);
		zVector seedPosTransDown = internalSeedPos + zVector(0, 0, -100);

		for (int j = 0; j < _mesh.faceNormals.size(); j++)
		{
			zVector tmpIntersectionPt;

			bool crit1 = line_PlaneIntersection(seedPosTransUp, seedPosTransDown, _mesh.faceNormals[j], _faceCenters[j], tmpIntersectionPt);

			vector<int> vertexIds;
			_mesh.getVertices(j, zFaceData, vertexIds);
			bool crit2 = pointInTriangle(tmpIntersectionPt, _mesh.vertexPositions[vertexIds[0]], _mesh.vertexPositions[vertexIds[1]], _mesh.vertexPositions[vertexIds[2]]);

			if (crit1 && crit2)
			{
				intersectionPt = tmpIntersectionPt;
				intersectionFaceId = j;
				break;
			}
		}

		// compute directional vector
		direction = _mesh.faceNormals[intersectionFaceId] ^ zVector(0.0, 0.0, 1.0);
		direction.normalize();
		direction = direction.rotateAboutAxis(_mesh.faceNormals[intersectionFaceId], _rotation);
		direction *= _stepDist;
		newIntersetionPt = intersectionPt + direction;

		// fill graphPostions container
		graphPositions.push_back(newIntersetionPt);
		// fill graphEdgeConnections container
		if (i != 0)
		{
			graphEdgeConnections.push_back(i - 1);
			graphEdgeConnections.push_back(i);
		}

		// update internalSeedPos
		internalSeedPos = newIntersetionPt;
	}

	// construct and return graph
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
vector<zGraph> slopeTraversalGraph;
vector<zVector> seedPos;

double rotation = 0.0; //-90 = rainflow
int numOfSteps = 200;
double stepDist = 1.0;


////// --- GUI OBJECTS ----------------------------------------------------
bool faceNormals = false;
bool showExistRoads = true;
bool showExistBuildings = true;
bool showSiteBoundary = false;



////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// enable smooth display
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);

	// import mesh
	fromOBJ(terrain, "data/turtlingBayPatch_terrain_tri.obj");
	getCenters(terrain, zFaceData, faceCenters);

	// color water and land
	for (int i = 0; i < terrain.vertexColors.size(); i++)
	{
		if (terrain.vertexPositions[i].z < -8.629) terrain.vertexColors[i] = zColor(0.0, 0.0, 0.5, 1.0);
		else terrain.vertexColors[i] = zColor(0.5, 0.5, 0.5, 1.0);
	}

	// import roads buildings etc.
	fromOBJ(existRoads, "data/turtlingBayPatch_streets.obj");
	setVertexColor(existRoads, zColor(0.0, 0.0, 0.0, 1.0), true);
	fromOBJ(existBuildings, "data/turtlingBayPatch_buildings.obj");
	setVertexColor(existBuildings, zColor(0.5, 0.5, 0.5, 1.0), true);
	fromOBJ(siteBoundary, "data/turtlingBayPatch_siteBoundary.obj");
	setVertexColor(siteBoundary, zColor(1, 0.0, (1 / 255 * 100), 1.0), true);
	
	// create buffer
	bufferObj = zBufferObject(1000000);
	bufferObj.appendMesh(terrain);


	// import seed points from txt
	fromTXT(seedPos, "data/turtlingBayPatch_seedPts.txt");

	
	for (int i = 0; i < seedPos.size(); i++)
		slopeTraversalGraph.push_back(getSlopeTraversalGraph(terrain, seedPos[i], faceCenters, rotation, stepDist, numOfSteps));


	// initialize buttonGroup
	B = *new ButtonGroup(vec(75, 20, 0));
	B.addButton(&faceNormals, "show face normals");
	B.addButton(&showExistRoads, "show exist roads");
	B.addButton(&showExistBuildings, "show exist buildings");
	B.addButton(&showSiteBoundary, "show site boundary");

	//// initialize sliderGourp
	//S = *new SliderGroup(vec(75, 200, 0));
	//S.addSlider(&rotation, "rotation");
	//S.addSlider(&stepDist, "step distance");
	//S.sliders[0].attachToVariable(&rotation, -180, 180);
	//S.sliders[1].attachToVariable(&stepDist, 0.5, 4);
}

void update(int value)
{
	//slopeTraversalGraph.clear();
	//for (int i = 0; i < seedPos.size(); i++)
	//	slopeTraversalGraph.push_back(getSlopeTraversalGraph(terrain, seedPos[1], faceCenters, rotation, stepDist, numOfSteps));
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
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
	for (int i = 0; i < seedPos.size(); i++)
	{
		drawPoint(seedPos[i], zColor(0, 0.5, 0, 1), 10);
		drawLine(seedPos[i], seedPos[i] + zVector(0, 0, -100));
	}

	// draw slope traversal graphs
	for (int i = 0; i < slopeTraversalGraph.size(); i++)
		drawGraph(slopeTraversalGraph[i], true, true);
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