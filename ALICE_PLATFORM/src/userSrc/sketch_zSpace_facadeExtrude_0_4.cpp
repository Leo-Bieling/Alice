
//#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v002
// zSpace header
#include <headers/app/include/zObjects.h>
#include <headers/app/include/zFnSets.h>
#include <headers/app/include/zViewer.h>

// zSpace toolset header
#include <headers/app/include/Tools/zTsGeometry.h>

using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
zModel model;

zObjMesh operateMesh;
zObjMesh offsetMesh;
zObjMesh thicknessMesh;

zFnMesh fnOperateMesh;
zFnMesh fnOffsetMesh;
zFnMesh fnThicknessMesh;

zTsVariableExtrude VE_operateMesh;
zTsVariableExtrude VE_offsetMesh;

string path = "data/variable_extrude_tester.json";

vector<int> edgeIds;

bool useBoundary = false;
bool useVertexColor = false;

bool offsetExtrude = false;
bool thicknessExtrude = false;

bool keepFacesTogether = true;
double minFaceOffset = 0.9;
double maxFaceOffset = 0.1;

double minFaceThickness = 0.1;
double maxFaceThickness = 0.5;

double faceWidthThreshold = 0.5;

double splitRatio = 0.2;

// double min_splitRatio = 0.1;
// double max_splitRatio = 0.4; // TODO implement splitRatio by color value

////// --- GUI OBJECTS ----------------------------------------------------
bool updateMeshTopo = false;
bool drawOperateMesh = true;
bool splitEdges = false;
bool reset = false;
bool exportResult = false;

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// create model buffer
	model = zModel(100000);

	// import json
	fnOperateMesh = zFnMesh(operateMesh);
	fnOperateMesh.from(path, zJSON);

	fnOffsetMesh = zFnMesh(offsetMesh);
	fnThicknessMesh = zFnMesh(thicknessMesh);

	// add mesh to model for display
	model.addObject(operateMesh);
	model.addObject(offsetMesh);
	model.addObject(thicknessMesh);

	// toggle display for reset
	thicknessMesh.setShowObject(false);
	operateMesh.setShowObject(true);
	offsetMesh.setShowObject(false);

	// clear meshes for reset
	fnOffsetMesh.clear();
	fnThicknessMesh.clear();
	
	////// --- GUI  ----------------------------------------------------

	// initialise button group
	B = *new ButtonGroup(vec(50, 500, 0));

	// assign and label buttons
	B.addButton(&updateMeshTopo, "update meshtopo");
	B.addButton(&drawOperateMesh, "show operate mesh");
	B.addButton(&offsetExtrude, "offset extrude");
	B.addButton(&thicknessExtrude, "thickness extrude");
	B.addButton(&useVertexColor, "use vertex color");
	B.addButton(&useBoundary, "use only boundary");
	B.addButton(&splitEdges, "split edges");
	B.addButton(&reset, "reset");
	B.addButton(&exportResult, "export");

	// initialise slider group
	S = *new SliderGroup(vec(50, 50, 0));

	// addign and label sliders
	S.addSlider(&minFaceOffset, "min offset");
	S.sliders[0].attachToVariable(&minFaceOffset, 0.0, 1.0);
	S.addSlider(&maxFaceOffset, "max offset");
	S.sliders[1].attachToVariable(&maxFaceOffset, 0.0, 1.0);
	S.addSlider(&minFaceThickness, "min thickness");
	S.sliders[2].attachToVariable(&minFaceThickness, 0.0, 10.0);
	S.addSlider(&maxFaceThickness, "max thickness");
	S.sliders[3].attachToVariable(&maxFaceThickness, 0.0, 10.0);
	S.addSlider(&splitRatio, "split ratio");
	S.sliders[4].attachToVariable(&splitRatio, 0.0, 0.49);
}

void update(int value)
{


	if (updateMeshTopo)
	{
		operateMesh.setShowObject(drawOperateMesh);
		
		// variable face offset
		if (offsetExtrude)
		{
			VE_operateMesh = zTsVariableExtrude(operateMesh);

			if (!useBoundary) 
				offsetMesh = VE_operateMesh.getVariableFaceOffset(keepFacesTogether, true, minFaceOffset, maxFaceOffset, useVertexColor);

			else 
				offsetMesh = VE_operateMesh.getVariableBoundaryOffset(keepFacesTogether, true, minFaceOffset, maxFaceOffset);

			offsetMesh.setShowObject(true);
			operateMesh.setShowObject(false);
			thicknessMesh.setShowObject(false);
		}


		// variable face extrude
		if (offsetExtrude && thicknessExtrude)
		{
			VE_offsetMesh = zTsVariableExtrude(offsetMesh);
			thicknessMesh = VE_offsetMesh.getVariableFaceThicknessExtrude(true, minFaceThickness, maxFaceThickness);
			
			thicknessMesh.setShowObject(true);
			operateMesh.setShowObject(false);
			offsetMesh.setShowObject(false);
		}

		if (!offsetExtrude && thicknessExtrude)
		{
			VE_offsetMesh = zTsVariableExtrude(offsetMesh);
			thicknessMesh = VE_offsetMesh.getVariableFaceThicknessExtrude(true, minFaceThickness, maxFaceThickness);

			thicknessMesh.setShowObject(true);
			operateMesh.setShowObject(false);
			offsetMesh.setShowObject(false);
		}

		// split edges // NOT WORKING YET
		if (splitEdges)
		{
			// getting edge Ids
			edgeIds.clear();
			for (int i = 0; i < fnOperateMesh.numEdges(); i++) edgeIds.push_back(i);
			
			// splitting edges
			float edgeLength1;
			float edgeLength2;
			float secondSplitRatio;
			int initialEdgeCount = fnOperateMesh.numEdges();
			int lastEdgeId;
			int tmpId;

			for (int i = 0; i < (initialEdgeCount / 2); i++)
			{
				// get edgeId for first split
				tmpId = i * 2;

				// first split
				fnOperateMesh.splitEdge(tmpId, 1 - splitRatio);
				// update lastEdgeId
				lastEdgeId = fnOperateMesh.numEdges() - 1;

				// compute second split ratio
				edgeLength1 = fnOperateMesh.getEdgelength(tmpId);
				edgeLength2 = fnOperateMesh.getEdgelength(lastEdgeId);
				secondSplitRatio = abs(edgeLength2 / edgeLength1);

				// second split
				fnOperateMesh.splitEdge(tmpId, secondSplitRatio);
			}

			splitEdges = !splitEdges;
		}

		updateMeshTopo = !updateMeshTopo;
	}

	// reset split mesh to original operateMesh
	if (reset)
	{
		setup();
		reset = !reset;
	}

	if (exportResult)
	{
		fnThicknessMesh.to("data/variable_extrude_result.json", zJSON);
		exportResult = !exportResult;
	}

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	//drawGrid(20.0);

	// draw buttons
	B.draw();

	// draw model
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

#endif _MAIN_