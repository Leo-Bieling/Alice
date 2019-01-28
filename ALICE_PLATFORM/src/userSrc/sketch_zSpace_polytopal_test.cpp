#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zData.h>
#include <headers/include/zIO.h>

#include <headers/application/zPolytopal.h>

using namespace zSpace;
using namespace std;

zColor computeOcculusionColor(zVector &norm, zVector &lightVec)
{
	double ang = norm.angle(lightVec);
	double val;

	if (ang <= 90) 	val = /*ofMap(ang, 0, 90, 1, 0.4)*/ 0.4;
	else if (ang > 90) 	val = ofMap(ang, 90, 180, 0.4, 1);

	zColor col;
	col = zColor(val, val, val, 1);

	return  col;
}

void getMayaData(int &_numberOfObjFiles, string &_workingDirectory)
{

	string inFileName = "data/app_polytopal/data.json";

	json j_in;
	ifstream in_myfile;

	in_myfile.open(inFileName.c_str());

	if (in_myfile.fail())
	{
		cout << " error in opening file  " << inFileName.c_str() << endl;
		return;
	}

	in_myfile >> j_in;
	in_myfile.close();

	// get polyhedra count	
	int polyhedraInputCount;
	polyhedraInputCount = (j_in["polyhedraCount"].get<int>());
	cout << "polyhedraInputCount: : " << polyhedraInputCount << endl;

	// get save directory
	string workingDirectory;
	workingDirectory = (j_in["workingDirectory"].get<string>());
	cout << "workingDirectory: " << workingDirectory << endl;

	_numberOfObjFiles = polyhedraInputCount;
	_workingDirectory = workingDirectory;
}

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
vector<zMesh> operateMeshes;
vector<double> offset;
zGraph centreLineGraph;
vector<zMesh> lowPolytopalMeshes;
vector<zMesh> smoothPolytopalMeshes;
vector<zMesh> temp;
vector<int> graphPosition_volumeMesh;
vector<int> graphPosition_volumeFace;
unordered_map <string, int> volumeFace_Vertex;
vector<double> graphVertexOffset;

int numberOfObjFiles;
string workingDirectory;

////// --- GUI OBJECTS ----------------------------------------------------
double background = 0.75;
double offSetValue = 0.05;

bool reimport = false;
bool drawOperateMeshes = true;
bool drawCentreLineGraph = false;
bool drawLowPolytopalMeshes = false;
bool drawSmoothPolytopalMeshes = false;
bool closeSmoothPolytopalMeshes = false;
bool exportMeshNodes = false;
bool exportMesh = false;
bool occlusion = false;
bool editOffset = false;

int smoothValue = 1;

zVector lightDirection = zVector(0.5, 0.5, 0.25);

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{	
	// get maya data
	getMayaData(numberOfObjFiles, workingDirectory);

	// import meshes from obj sequence
	for (int i = 0; i < numberOfObjFiles; i++)
	{
		zMesh tmpMesh;
		fromOBJ(tmpMesh, ("data/app_polytopal/mesh_" + to_string(i) + ".obj"));
		operateMeshes.push_back(tmpMesh);
	}

	// create center line graph with preset offset
	for (int i = 0; i < numberOfObjFiles; i++)
		offset.push_back(offSetValue);

	centreLineGraph = createCentreLineGraph(operateMeshes, offset, volumeFace_Vertex, graphPosition_volumeMesh, graphPosition_volumeFace, graphVertexOffset);

	// add sliders
	S = *new SliderGroup();
	S.addSlider(&background, "background");
	S.addSlider(&offSetValue, "offSetValue");
	S.sliders[1].attachToVariable(&offSetValue, 0.001, 0.999);

	// add buttons
	B = *new ButtonGroup(vec(100, 100, 0));
	B.addButton(&reimport, "Reimport meshes");	
	B.addButton(&editOffset, "Edit offset value");
	B.addButton(&drawOperateMeshes, "Operate meshes");
	B.addButton(&drawCentreLineGraph, "Centre line graph");
	B.addButton(&drawLowPolytopalMeshes, "Low polytopal meshes");
	B.addButton(&drawSmoothPolytopalMeshes, "Smooth polytopal meshes");
	B.addButton(&closeSmoothPolytopalMeshes, "Close nodes");
	B.addButton(&occlusion, "Display occlusion");
	B.addButton(&exportMeshNodes, "Export mesh nodes");
	B.addButton(&exportMesh, "Export mesh");
}

void update(int value)
{
	// reimport operateMesh if toggled
	if (reimport)
	{
		getMayaData(numberOfObjFiles, workingDirectory);

		operateMeshes.clear();

		for (int i = 0; i < numberOfObjFiles; i++)
		{
			zMesh tmpMesh;
			fromOBJ(tmpMesh, ("data/app_polytopal/mesh_" + to_string(i) + ".obj"));
			operateMeshes.push_back(tmpMesh);
		}
		reimport = !reimport;
	}

	// update center line graph with new offset
	if (editOffset)
	{
		offset.clear();
		for (int i = 0; i < numberOfObjFiles; i++)
			offset.push_back(offSetValue);

		centreLineGraph = createCentreLineGraph(operateMeshes, offset, volumeFace_Vertex, graphPosition_volumeMesh, graphPosition_volumeFace, graphVertexOffset);
	}

	// create low_polytopal mesh
	lowPolytopalMeshes.clear();
	smoothPolytopalMeshes.clear();

	for (int i = 0; i < operateMeshes.size(); i++)
	{
		zMesh lowPolytopalMesh = createPolytopalMesh(operateMeshes[i], i, centreLineGraph, volumeFace_Vertex, graphVertexOffset);

		zMesh smoothPolytopalMesh = createPolytopalMesh(operateMeshes[i], i, centreLineGraph, volumeFace_Vertex, graphVertexOffset);

		lowPolytopalMeshes.push_back(lowPolytopalMesh);
		smoothPolytopalMeshes.push_back(smoothPolytopalMesh);
	}

	// smoothing mesh
	if (drawSmoothPolytopalMeshes)
	{
		temp.clear();
		for (int i = 0; i < operateMeshes.size(); i++)
		{
			// remesh to remove ruling along the direction of the strips 
			zMesh smooth_polytopalMesh = remeshSmoothPolytopalMesh(lowPolytopalMeshes[i], smoothPolytopalMeshes[i], smoothValue);

			smoothPolytopalMeshes[i] = smooth_polytopalMesh;
		}
	}

	// close smoothed meshes
	if (closeSmoothPolytopalMeshes)
	{
		for (int i = 0; i < operateMeshes.size(); i++)
			closePolytopalMesh(operateMeshes[i], smoothPolytopalMeshes[i]);
	}

	// add occlusion
	if (occlusion)
	{
		// low poly mesh
		for (int i = 0; i < lowPolytopalMeshes.size(); i++)
		{
			lowPolytopalMeshes[i].computeMeshNormals();

			for (int j = 0; j < lowPolytopalMeshes[i].faceNormals.size(); j++)
				lowPolytopalMeshes[i].faceColors[j] = computeOcculusionColor(lowPolytopalMeshes[i].faceNormals[j], lightDirection);
		}

		// smooth mesh
		for (int i = 0; i < smoothPolytopalMeshes.size(); i++)
		{
			smoothPolytopalMeshes[i].computeMeshNormals();

			for (int j = 0; j < smoothPolytopalMeshes[i].faceNormals.size(); j++)
				smoothPolytopalMeshes[i].faceColors[j] = computeOcculusionColor(smoothPolytopalMeshes[i].faceNormals[j], lightDirection);
		}
	}

	// export mesh node by node
	if (exportMeshNodes)
	{
		if (drawLowPolytopalMeshes) 
			for (int i = 0; i < lowPolytopalMeshes.size(); i++) 
				toOBJ(lowPolytopalMeshes[i], ("data/app_polytopal/output/lowPolytopalMesh_" + to_string(i) + ".obj"));
		if (drawSmoothPolytopalMeshes)
			for (int i = 0; i < smoothPolytopalMeshes.size(); i++) 
				toOBJ(smoothPolytopalMeshes[i], ("data/app_polytopal/output/smoothPolytopalMesh_" + to_string(i) + ".obj"));
		
		exportMeshNodes = !exportMeshNodes;
	}

	// export mesh
	if (exportMesh)
	{
		if (drawLowPolytopalMeshes)
		{
			zMesh combinedLowPolytopalMesh;
			for (int i = 0; i < lowPolytopalMeshes.size(); i++)
				combinedLowPolytopalMesh = combineDisjointMesh(combinedLowPolytopalMesh, lowPolytopalMeshes[i]);

			toOBJ(combinedLowPolytopalMesh, ("data/app_polytopal/output/combinedLowPolytopalMesh.obj"));
		}
		
		if (drawSmoothPolytopalMeshes)
		{
			zMesh combinedSmoothPolytopalMesh;
			for (int i = 0; i < lowPolytopalMeshes.size(); i++)
				combinedSmoothPolytopalMesh = combineDisjointMesh(combinedSmoothPolytopalMesh, lowPolytopalMeshes[i]);

			toOBJ(combinedSmoothPolytopalMesh, ("data/polytopal/app_output/combinedSmoothPolytopalMesh.obj"));
		}
		exportMesh = !exportMesh;
	}
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(background);
	drawGrid(20.0);

	// draw operateMeshes
	if(drawOperateMeshes) for (int i = 0; i < operateMeshes.size(); i++) drawMesh(operateMeshes[i], false, true, false);

	// draw centreLineGraph
	if (drawCentreLineGraph) drawGraph(centreLineGraph, true, true);

	// draw lowPolytopalMeshes
	if (drawLowPolytopalMeshes) for (int i = 0; i < lowPolytopalMeshes.size(); i++) drawMesh(lowPolytopalMeshes[i], false, true, true);

	// draw smoothPolytopalMeshes
	if (drawSmoothPolytopalMeshes) for (int i = 0; i < smoothPolytopalMeshes.size(); i++) drawMesh(smoothPolytopalMeshes[i], false, true, true);
	
	// draw string
	setup2d();
	drawString("Press 'q' to increase smoothness", vec(100, 800, 0));
	drawString("Press 'a' to decrease smoothness", vec(100, 825, 0));
	restore3d();
}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'q') if (smoothValue < 3) smoothValue++;
	if (k == 'a') if (smoothValue >= 1) smoothValue--;
}

void mousePress(int b, int state, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif // _MAIN_