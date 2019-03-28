#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zData.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zIO.h>
#include <headers/application/zVariableExtrude.h>



using namespace zSpace;
using namespace std;

// VARIABLES

zMesh operateMesh;
zMesh offsetMesh;
zMesh thicknessMesh;

vector<int> edgeIds;

bool useONLYBOUNDARY = false;
bool useVERTEXCOLOR = false;

bool offsetEXTRUDE = true;
bool thicknessEXTRUDE = true;

bool keepFACES = true;
double min_FACEOFFSET = 0.9;
double max_FACEOFFSET = 0.1;

double min_FACETHICKNESS = 0.1;
double max_FACETHICKNESS = 0.5;

double face_WIDTH_THRESHOLD = 0.5;

double splitRatio = 0.2;
// double min_splitRatio = 0.1;
// double max_splitRatio = 0.4; // TODO implement splitRatio by color value

bool updateMESHTOPOLOGY = false;
bool drawOperateMesh = true;
bool drawOffsetMesh = false;
bool drawThicknessMesh = false;
bool splitEdges = false;
bool reset = false;
bool exportResult = false;

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// import obj
	fromJSON(operateMesh, "data/facade_from_maya.json");

	//// detach every face in the mesh
	//vector<zVector> positionsTmp;
	//vector<zVector> positions;
	//vector<int> polyCounts;
	//vector<int> polyConnections;

	//for (int i = 0; i < operateMesh.numPolygons(); i++)
	//{
	//	int currentVertNum = positions.size();
	//	operateMesh.getVertexPositions(i, zFaceData, positionsTmp); // why is positionsTmp not getting pushed back? would only need one vector zVector
	//	for (int i = 0; i < positionsTmp.size(); i++) positions.push_back(positionsTmp[i]);
	//	
	//	for (int j = 0; j < (positions.size() - currentVertNum); j++) polyConnections.push_back(currentVertNum + j);

	//	polyCounts.push_back(positions.size() - currentVertNum);
	//}

	//operateMesh = zMesh(positions, polyCounts, polyConnections);



	// initialise button group
	B = *new ButtonGroup(vec(50, 500, 0));

	// assign and label buttons
	B.addButton(&updateMESHTOPOLOGY, "update meshtopo");
	B.addButton(&drawOperateMesh, "draw operate mesh");
	B.addButton(&drawOffsetMesh, "draw offset mesh");
	B.addButton(&drawThicknessMesh, "draw thickness mesh");
	B.addButton(&offsetEXTRUDE, "offset extrude");
	B.addButton(&thicknessEXTRUDE, "thickness extrude");
	B.addButton(&useVERTEXCOLOR, "use vertex color");
	B.addButton(&useONLYBOUNDARY, "use only boundary");
	B.addButton(&splitEdges, "split edges");
	B.addButton(&reset, "reset");
	B.addButton(&exportResult, "export");

	// initialise slider group
	S = *new SliderGroup(vec(50, 50, 0));

	// addign and label sliders
	S.addSlider(&min_FACEOFFSET, "min offset");
	S.sliders[0].attachToVariable(&min_FACEOFFSET, 0.0, 1.0);
	S.addSlider(&max_FACEOFFSET, "max offset");
	S.sliders[1].attachToVariable(&max_FACEOFFSET, 0.0, 1.0);
	S.addSlider(&min_FACETHICKNESS, "min thickness");
	S.sliders[2].attachToVariable(&min_FACETHICKNESS, 0.0, 10.0);
	S.addSlider(&max_FACETHICKNESS, "max thickness");
	S.sliders[3].attachToVariable(&max_FACETHICKNESS, 0.0, 10.0);
	S.addSlider(&splitRatio, "split ratio");
	S.sliders[4].attachToVariable(&splitRatio, 0.0, 0.49);
}

void update(int value)
{
	if (updateMESHTOPOLOGY)
	{

		// split edges
		if (splitEdges)
		{
			// getting edge Ids
			edgeIds.clear();
			for (int i = 0; i < operateMesh.numEdges(); i++) edgeIds.push_back(i);

			// splitting edges
			float edgeLength1;
			float edgeLength2;
			float secondSplitRatio;
			int initialEdgeCount = operateMesh.numEdges();
			int lastEdgeId;
			int tmpId;

			for (int i = 0; i < (initialEdgeCount / 2); i++)
			{
				// get edgeId for first split
				tmpId = i * 2;

				// first split
				splitEdge(operateMesh, tmpId, 1 - splitRatio);

				// update lastEdgeId
				lastEdgeId = operateMesh.numEdges() - 1;

				// compute second split ratio
				edgeLength1 = getEdgelength(operateMesh, tmpId);
				edgeLength2 = getEdgelength(operateMesh, lastEdgeId);
				secondSplitRatio = abs(edgeLength2 / edgeLength1);

				// second split
				splitEdge(operateMesh, tmpId, secondSplitRatio);
			}

			splitEdges = !splitEdges;
		}

		// reset split mesh to original operateMesh
		if (reset)
		{
			setup();
			reset = !reset;
		}

		// variable face offset
		if (offsetEXTRUDE)
		{
			offsetMesh.clear();
			if (!useONLYBOUNDARY) offsetMesh = variableFaceOffsetExtrude(operateMesh, keepFACES, true, min_FACEOFFSET, max_FACEOFFSET, useVERTEXCOLOR);

			else offsetMesh = variableBoundaryOffset(operateMesh, keepFACES, true, min_FACEOFFSET, max_FACEOFFSET);
		}


		// variable face extrude
		if (offsetEXTRUDE && thicknessEXTRUDE)
		{
			thicknessMesh.clear();
			thicknessMesh = variableFaceThicknessExtrude(offsetMesh, true, min_FACETHICKNESS, max_FACETHICKNESS);
		}

		if (!offsetEXTRUDE && thicknessEXTRUDE)
		{
			thicknessMesh.clear();
			thicknessMesh = variableFaceThicknessExtrude(operateMesh, true, min_FACETHICKNESS, max_FACETHICKNESS);
		}

		updateMESHTOPOLOGY = !updateMESHTOPOLOGY;
	}

	if (exportResult)
	{
		toJSON(thicknessMesh, "C:/Users/Leo.b/Desktop/result_from_alice.json", true);
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

	// draw meshes
	if (drawOperateMesh) drawMesh(operateMesh, true, true, true);
	if (drawOffsetMesh) drawMesh(offsetMesh, true, true, true);
	if (drawThicknessMesh) drawMesh(thicknessMesh, true, true, true);
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