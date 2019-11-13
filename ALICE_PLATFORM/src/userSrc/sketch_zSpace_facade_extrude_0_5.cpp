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


string path = "data/hs_comp_facade_family.json";

vector<int> edgeIds;

bool useBoundary = false;
bool useVertexColor = false;

bool offsetExtrude = false;
bool thicknessExtrude = false;

bool keepFacesTogether = true;
double minFaceOffset = 0.9;
double maxFaceOffset = 0.1;

double minFaceThickness = 0.4;
double maxFaceThickness = 2.0;

double faceWidthThreshold = 0.5;

int numberOfFamilies = 3;


////// --- GUI OBJECTS ----------------------------------------------------
bool updateMeshTopo = false;
bool drawOperateMesh = true;
bool downsample = false;
bool showFaceNormals = false;
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
	fnOperateMesh.computeFaceColorfromVertexColor();

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
	B.addButton(&updateMeshTopo, "update");
	B.addButton(&drawOperateMesh, "show operate mesh");
	B.addButton(&offsetExtrude, "offset extrude");
	B.addButton(&thicknessExtrude, "thickness extrude");
	B.addButton(&useVertexColor, "use vertex color");
	B.addButton(&useBoundary, "use only boundary");
	B.addButton(&downsample, "downsample");
	B.addButton(&showFaceNormals, "show face normals");
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
}

void update(int value)
{
	
	vector<zVector> faceCenters;
	fnOperateMesh.getCenters(zFaceData, faceCenters);
	operateMesh.setShowFaceNormals(showFaceNormals, faceCenters, 2);

	//if(showFaceNormals) operateMesh.drawMesh_FaceNormals();

	if (downsample)
	{
		setup();

		// downsample mapdata
		vector<double> sampleValues;
		double sampleDelta = 1.0 / numberOfFamilies;

		// get new sampleValues / steps
		for (int i = 1; i <= numberOfFamilies; i++)
			sampleValues.push_back(sampleDelta * i);

		// compute deltas from mapdata to new sampleValues and assign new faceColor
		double delta;
		for (int i = 0; i < sampleValues.size(); i++)
			for (int j = 0; j < fnOperateMesh.numPolygons(); j++)
			{
				delta = fnOperateMesh.getFaceColor(j).r - sampleValues[i];

				if (delta < sampleDelta && delta > 0)
					fnOperateMesh.setFaceColor(j, zColor(sampleValues[i + 1], sampleValues[i + 1], sampleValues[i + 1], 1.0));

				if (fnOperateMesh.getFaceColor(j).r == 0)
					fnOperateMesh.setFaceColor(j, zColor(sampleValues[0], sampleValues[0], sampleValues[0], 1.0));

				if (fnOperateMesh.getFaceColor(j).r < sampleValues[0])
					fnOperateMesh.setFaceColor(j, zColor(sampleValues[0], sampleValues[0], sampleValues[0], 1.0));
			}

		downsample = !downsample;
	}

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

			// set faceColor to grey - tmp
			fnOffsetMesh.setFaceColor(zColor(0.5, 0.5, 0.5, 1.0));

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

		// set faceColor to grey - tmp
		fnThicknessMesh.setFaceColor(zColor(0.5, 0.5, 0.5, 1.0));

		offsetExtrude = false;
		thicknessExtrude = false;
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

	B.draw();
	S.draw();

	model.draw();

	// draw string
	setup2d();
	drawString("Press 'q' to increase number of families", vec(50, 250, 0));
	drawString("Press 'a' to decrease number of families", vec(50, 275, 0));
	drawString(("number of family types: " + to_string(numberOfFamilies)), vec(50, 300, 0));
	restore3d();
}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'q') numberOfFamilies++;
	if (k == 'a') numberOfFamilies--;		
}

void mousePress(int b, int state, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif _MAIN_