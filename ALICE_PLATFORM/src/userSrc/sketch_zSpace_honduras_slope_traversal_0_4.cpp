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

zGraph getSlopeTraversalGraph(zMesh &_mesh, zField2D<zVector> &_field, int _n_X, zVector &_seedPos, vector<zVector> &_faceCenters, double _rotation, double _stepDist, int _numOfSteps)
{
	int intersectionFaceId;
	zVector internalSeedPos = _seedPos;
	zVector intersectionPt;
	zVector newIntersetionPt;
	zVector direction;

	vector<zVector> graphPositions;
	vector<int> graphEdgeConnections;

	int idX, idY;
	int id;
	int ceMeshId;



	for (int i = 0; i < _numOfSteps; i++)
	{
		// match indices
		id = _field.getIndex(internalSeedPos);
		_field.getIndices(internalSeedPos, idX, idY);

		 ceMeshId = idY * _n_X + idX;

		// get intersection point with quad face
		zVector seedPosTransUp = internalSeedPos + zVector(0, 0, 100);
		zVector seedPosTransDown = internalSeedPos + zVector(0, 0, -100);
		bool check = line_PlaneIntersection(seedPosTransUp, seedPosTransDown, _mesh.faceNormals[ceMeshId], _faceCenters[ceMeshId], intersectionPt);
		
		if (check)
		{
			// compute directional vector
			direction = _mesh.faceNormals[ceMeshId] ^ zVector(0.0, 0.0, 1.0);
			direction.normalize();
			direction = direction.rotateAboutAxis(_mesh.faceNormals[ceMeshId], _rotation);
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
zMesh fieldMesh;
zField2D<zVector> field;
int n_X;
int n_Y;

double rotation = 0.0; //-90.0 = rainflow
int numOfSteps = 50;
double stepDist = 1.0;



////// --- GUI OBJECTS ----------------------------------------------------
bool showTerrainMesh = true;
bool showFaceNormals = false;
bool showField = false;
bool showExistRoads = false;
bool showExistBuildings = false;
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
	fromOBJ(terrain, "data/roatan_terrain.obj");
	getCenters(terrain, zFaceData, faceCenters);

	// import seed points from txt
	fromTXT(seedPos, "data/roatan_seedPts.txt");

	// construct field
	//// get boundary max and min location
	vector<double> meshX_values;
	vector<double> meshY_values;

	for (int i = 0; i < terrain.numVertices(); i++)
	{
		meshX_values.push_back(terrain.vertexPositions[i].x);
		meshY_values.push_back(terrain.vertexPositions[i].y);
	}
	zVector minBB = zVector(zMin(meshX_values), zMax(meshY_values), 0);
	zVector maxBB = zVector(zMax(meshX_values), zMin(meshY_values), 0);

	//// get step size
	vector<int> vertexIds;
	terrain.getVertices(0, zFaceData, vertexIds);
	vector<double> faceX_values;
	vector<double> faceY_values;
	for (int i = 0; i < vertexIds.size(); i++)
	{
		faceX_values.push_back(terrain.vertexPositions[vertexIds[i]].x);
		faceY_values.push_back(terrain.vertexPositions[vertexIds[i]].y);
	}
	double xLength = abs(zMin(meshX_values)) + abs(zMax(meshX_values));
	double yLength = abs(zMin(meshY_values)) + abs(zMax(meshY_values));
	double stepSizeX = abs(zMin(faceX_values)) - abs(zMax(faceX_values));
	double stepSizeY = abs(zMax(faceY_values) - zMin(faceY_values));
	n_X = xLength / stepSizeX + 1;
	n_Y = yLength / stepSizeY + 1;

	//// initialise field
	field = zField2D<zVector>(minBB, maxBB,  n_X,  n_Y);
	//// create mesh from field for display
	from2DFIELD(fieldMesh, field);
	

	// color water and land
	for (int i = 0; i < terrain.vertexColors.size(); i++)
	{
		if (terrain.vertexPositions[i].z < -8.629) terrain.vertexColors[i] = zColor(0.0, 0.0, 0.5, 1.0);
		else terrain.vertexColors[i] = zColor(0.5, 0.5, 0.5, 1.0);
	}

	// import roads buildings etc.
	fromOBJ(existRoads, "data/roatan_streets.obj");
	setVertexColor(existRoads, zColor(0.0, 0.0, 0.0, 1.0), true);
	fromOBJ(existBuildings, "data/roatan_buildings.obj");
	setVertexColor(existBuildings, zColor(0.5, 0.5, 0.5, 1.0), true);
	fromOBJ(siteBoundary, "data/roatan_siteBoundary.obj");
	setVertexColor(siteBoundary, zColor(1, 0.0, (1 / 255 * 100), 1.0), true);
	
	// create buffer
	bufferObj = zBufferObject(1000000);
	bufferObj.appendMesh(terrain);

	// initialize buttonGroup
	B = *new ButtonGroup(vec(75, 20, 0));
	B.addButton(&showTerrainMesh, "show terrain");
	B.addButton(&showFaceNormals, "show face normals");
	B.addButton(&showField, "show field");
	B.addButton(&showExistRoads, "show exist roads");
	B.addButton(&showExistBuildings, "show exist buildings");
	B.addButton(&showSiteBoundary, "show site boundary");

	// initialize sliderGourp
	S = *new SliderGroup(vec(75, 400, 0));
	S.addSlider(&rotation, "rotation");
	S.addSlider(&stepDist, "step distance");
	S.sliders[0].attachToVariable(&rotation, -180, 180);
	S.sliders[1].attachToVariable(&stepDist, 0.5, 4);
}

void update(int value)
{
	slopeTraversalGraph.clear();
	for (int i = 0; i < seedPos.size(); i++)
	{
		slopeTraversalGraph.push_back(getSlopeTraversalGraph(terrain, field, n_X, seedPos[i], faceCenters, rotation, stepDist, numOfSteps));
		slopeTraversalGraph.push_back(getSlopeTraversalGraph(terrain, field, n_X, seedPos[i], faceCenters, rotation - 180, stepDist, numOfSteps));
	}
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	//drawGrid(20.0);

	// draw from buffer
	if(showTerrainMesh)
		drawLinesFromBuffer(bufferObj);

	// draw meshes
	if(showExistBuildings) 
		drawMesh(existBuildings, false, true, true);
	if(showExistRoads) 
		drawMesh(existRoads, false, false, true);
	if (showSiteBoundary)
		drawMesh(siteBoundary, false, true, true);

	// draw face normals
	if (showFaceNormals) 
		drawMesh_FaceNormals(terrain, faceCenters, 2.0);

	// draw seed points
	for (int i = 0; i < seedPos.size(); i++)
	{
		drawPoint(seedPos[i], zColor(0, 0.5, 0, 1), 10);
		drawLine(seedPos[i], seedPos[i] + zVector(0, 0, -50));
	}
	
	if (showField)
		drawMesh(fieldMesh, false, true, true);

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