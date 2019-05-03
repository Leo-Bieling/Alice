//#define _MAIN_

#ifdef _MAIN_

#include "main.h"
// zSpace v001
#include <headers/include/zCore.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zDynamics.h>
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

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
vector<zMesh> volumeForceMeshes;
vector<double> offsets;
zGraph formGraph;

vector<zMesh> lowPolytopalMeshes;
vector<zMesh> smoothPolytopalMeshes;

vector<int> formGraphVertex_forceVolumeMesh;
vector<int> formGraphVertex_forceVolumeFace;
unordered_map <string, int> forceVolumeFace_formGraphVertex;
vector<double> formGraphVertexOffsets;

vector<vector<zVector>> forceVolumeMeshFaceCenters;
vector<zParticle> formGraphParticles;

//string workingDirectory = "//zaha-hadid.com/Data/Projects/1453_CODE/1453___research/res_leo/research/DMS_paper_coupled_system/obj/all_polyhedra";
string workingDirectory = "//zaha-hadid.com/Data/Projects/1453_CODE/1453___research/res_leo/research/DMS_paper_coupled_system/obj/beijing_prototype_polyhedra";
int numberOfObjFiles = getNumfiles_Type(workingDirectory, "obj");


////// --- GUI OBJECTS ----------------------------------------------------
double background = 0.75;
double offSetValue = 0.05;

bool reimport = false;
bool drawvolumeForceMeshes = true;
bool drawformGraph = false;
bool drawLowPolytopalMeshes = false;
bool drawSmoothPolytopalMeshes = false;
bool closeSmoothPolytopalMeshes = false;
bool exportMeshNodes = false;
bool exportMesh = false;
bool occlusion = false;
bool editOffset = false;

bool compressionNetwork = false;

int smoothValue = 1;

zVector lightDirection = zVector(0.5, 0.5, 0.25);
vec camPt;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{	
	// import meshes from obj sequence
	for (int i = 0; i < numberOfObjFiles; i++)
	{
		zMesh tmpMesh;
		fromOBJ(tmpMesh, (workingDirectory + "/mesh_" + to_string(i) + ".obj"));

		//printMesh(tmpMesh);
		//printf("\n faceActive: %i ", tmpMesh.faceActive.size());

		volumeForceMeshes.push_back(tmpMesh);
	}

	// create form graph with preset offsets
	for (int i = 0; i < numberOfObjFiles; i++)
		offsets.push_back(offSetValue);

	formGraph = createFormGraph(volumeForceMeshes, offsets, forceVolumeFace_formGraphVertex, formGraphVertex_forceVolumeMesh, formGraphVertex_forceVolumeFace, formGraphVertexOffsets);
	
	// compute facecenters of forceVolumeMeshes
	getForceVolumeMesh_FaceCenters(volumeForceMeshes,forceVolumeMeshFaceCenters);
	
	

	// add sliders
	S = *new SliderGroup();
	S.addSlider(&background, "background");
	S.addSlider(&offSetValue, "offSetValue");
	S.sliders[1].attachToVariable(&offSetValue, 0.001, 0.999);

	// add buttons
	B = *new ButtonGroup(vec(100, 100, 0));
	B.addButton(&reimport, "Reimport meshes");	
	B.addButton(&editOffset, "Edit offset & reset");
	B.addButton(&drawvolumeForceMeshes, "Volume force meshes");
	B.addButton(&drawformGraph, "Centre line graph");
	B.addButton(&drawLowPolytopalMeshes, "Low polytopal meshes");
	B.addButton(&drawSmoothPolytopalMeshes, "Smooth polytopal meshes");
	B.addButton(&closeSmoothPolytopalMeshes, "Close nodes");
	B.addButton(&occlusion, "Display occlusion");
	B.addButton(&exportMeshNodes, "Export node meshes");
	B.addButton(&exportMesh, "Export overall mesh");

	B.addButton(&compressionNetwork, "Compression network");
}

void update(int value)
{
	// reimport operateMesh if toggled
	if (reimport)
	{
		numberOfObjFiles = getNumfiles_Type(workingDirectory, "obj");

		volumeForceMeshes.clear();

		for (int i = 0; i < numberOfObjFiles; i++)
		{
			zMesh tmpMesh;
			fromOBJ(tmpMesh, ("data/app_polytopal/mesh_" + to_string(i) + ".obj"));

			volumeForceMeshes.push_back(tmpMesh);
		}
		reimport = !reimport;
	}

	// update center line graph with new offsets
	if (editOffset)
	{
		offsets.clear();
		for (int i = 0; i < numberOfObjFiles; i++)
			offsets.push_back(offSetValue);

		formGraph = createFormGraph(volumeForceMeshes, offsets, forceVolumeFace_formGraphVertex, formGraphVertex_forceVolumeMesh, formGraphVertex_forceVolumeFace, formGraphVertexOffsets);
	}


	// create low_polytopal mesh
	for (int i = 0; i < lowPolytopalMeshes.size(); i++) lowPolytopalMeshes[i].clear(); // clearing dynamic memory of the mesh
	lowPolytopalMeshes.clear(); // clearing array

	for (int i = 0; i < smoothPolytopalMeshes.size(); i++) smoothPolytopalMeshes[i].clear(); // clearing dynamic memory of the mesh
	smoothPolytopalMeshes.clear(); // clearing array

	for (int i = 0; i < volumeForceMeshes.size(); i++)
	{
		zMesh lowPolytopalMesh = createPolytopalMesh(volumeForceMeshes[i], i, formGraph, forceVolumeFace_formGraphVertex, formGraphVertexOffsets);

		zMesh smoothPolytopalMesh = createPolytopalMesh(volumeForceMeshes[i], i, formGraph, forceVolumeFace_formGraphVertex, formGraphVertexOffsets);

		lowPolytopalMeshes.push_back(lowPolytopalMesh);
		smoothPolytopalMeshes.push_back(smoothPolytopalMesh);
	}

	// smoothing mesh

	if (drawSmoothPolytopalMeshes)
	{		
		for (int i = 0; i < volumeForceMeshes.size(); i++)
		{
			// remesh to remove ruling along the direction of the strips 
			zMesh smooth_polytopalMesh = remeshSmoothPolytopalMesh(smoothPolytopalMeshes[i], smoothValue);

			smoothPolytopalMeshes[i] = smooth_polytopalMesh;
		}
	}

	// close smoothed meshes
	if (closeSmoothPolytopalMeshes)
	{
		for (int i = 0; i < volumeForceMeshes.size(); i++)
			closePolytopalMesh(volumeForceMeshes[i], smoothPolytopalMeshes[i]);
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
				toOBJ(lowPolytopalMeshes[i], (workingDirectory + "/output/lowPolytopalMesh_" + to_string(i) + ".obj"));
		if (drawSmoothPolytopalMeshes)
			for (int i = 0; i < smoothPolytopalMeshes.size(); i++) 
				toOBJ(smoothPolytopalMeshes[i], (workingDirectory + "/output/smoothPolytopalMesh_" + to_string(i) + ".obj"));
		
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

			toOBJ(combinedLowPolytopalMesh, (workingDirectory + "/output/combinedLowPolytopalMesh.obj"));
		}
		
		if (drawSmoothPolytopalMeshes)
		{
			zMesh combinedSmoothPolytopalMesh;
			for (int i = 0; i < lowPolytopalMeshes.size(); i++)
				combinedSmoothPolytopalMesh = combineDisjointMesh(combinedSmoothPolytopalMesh, lowPolytopalMeshes[i]);

			toOBJ(combinedSmoothPolytopalMesh, (workingDirectory + "/output/combinedSmoothPolytopalMesh.obj"));
		}
		exportMesh = !exportMesh;
	}


	// compression network
	if (compressionNetwork)
	{
		bool temp = updateFormGraph(volumeForceMeshes, formGraph, forceVolumeMeshFaceCenters, formGraphVertex_forceVolumeMesh, formGraphVertex_forceVolumeFace, formGraphParticles, 1.0, zRK4);

		compressionNetwork = !temp;

		if (!compressionNetwork) computeFormGraph_EdgeWeights(volumeForceMeshes, formGraph, formGraphVertex_forceVolumeMesh, formGraphVertex_forceVolumeFace);

		//printf("\n compNetwork: %s ", (compressionNetwork) ? "true" : "false");
	}
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(background);
	drawGrid(20.0);

	// draw volumeForceMeshes
	if (drawvolumeForceMeshes)
	{
		for (int i = 0; i < volumeForceMeshes.size(); i++)
		{
			drawMesh(volumeForceMeshes[i], false, true, false);

			vector<zVector> fCenters;
			getCenters(volumeForceMeshes[i], zFaceData, fCenters);

			//drawMesh_FaceNormals(volumeForceMeshes[i], fCenters);
		}
	}

	// draw formGraph
	if (drawformGraph) drawGraph(formGraph, true, true);

	// draw lowPolytopalMeshes
	if (drawLowPolytopalMeshes)
	{
		for (int i = 0; i < lowPolytopalMeshes.size(); i++)
		{
			drawMesh(lowPolytopalMeshes[i], false, true, true);		
		}
	}

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