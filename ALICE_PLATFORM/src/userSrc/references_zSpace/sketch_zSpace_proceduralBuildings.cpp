#define _MAIN_

#ifdef _MAIN_

#include "main.h"

//////  zSpace Library
#include <zSpace/zSpaceImplement.h>
#include <zSpace/zMeshUtilities.h>
#include <zSpace/zProceduralBuilding.h>
#include <zSpace/zDisplay.h>


using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------

double increment = 1;

zBufferObject bufferObj;

vector<zBufferObject> floorBuffer;
vector<zMesh> operateMesh;

zProceduralBuilding pBuilding;

vector<vector<double>> scalarValues;

bool importPlot = false;;
bool setBOffset = false;;
bool addFloor = false;;



bool generateFloor_GFA = false;

// display booleans
bool dField = false;
bool dBuffer = true;

bool editFloor = false;;
double floorId = 0;

double plotOffset = 5; 
double offsetWeight = 0.425;

double floor_numSplits = 1.0;

double terrace_numVerts = 20.0;
double terrace_gap_numVerts = 0.0;
double terrace_numSplits = 2.0;
double terrace_startId = 0.0;

double balcony_numVerts = 3.0;
double balcony_gap_numVerts = 1.0;
double balcony_numSplits = 1.0;
double balcony_startId = 0.0;

double bridge_num = 1.0;
double bridge_gap_numVerts = 3.0;
double bridge_numSplits = 1.0;
double bridge_startId = 0.0;

double requiredGFA = 1000;

bool run = false;
bool updateColors = false;
int rCnt = 0;
////// --- GUI OBJECTS ----------------------------------------------------

zGraph terraceGraph;
zGraph balconyGraph;
zGraph slabGraph;
zGraph bridgeGraph;


char s[200],text[200], text1[200], jts[400];

double background = 0.9;

vec camPt;
////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------

//largeMesh LM;
int fileNum = 0;
string printfile;
////// ---------------------------------------------------- MODEL  ----------------------------------------------------


void setup()
{

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	//////////////////////////////////////////////////////////////////////////


	bufferObj = zBufferObject(1000000);
	
	zVector minBB(-100, -100,0);
	zVector maxBB(100, 100, 0);

	int resX = 100; 
	int resY = 100;

	pBuilding.field = zScalarField2D(minBB, maxBB, resX, resY);
	pBuilding.fieldMesh = createFieldMesh(pBuilding.field);

	//vector<zVector> zDistPts = {zVector(80,80,0)};	
	//static vector<double> sValues;
	//assignScalarsAsVertexDistance(pBuilding.fieldMesh, zDistPts, sValues);

	
	bufferObj.appendMesh(pBuilding.fieldMesh);
	
	
	pBuilding.maxHeight = 60;
	pBuilding.floorWidth = 15;
	pBuilding.floorHeight = 6;
	pBuilding.slabDepth = 0.5;
	pBuilding.terraceWidth = 1.5;
	pBuilding.balconyWidth = 3.0;
	pBuilding.bridgeWidth = 15;

	//contourMesh = scalarField.isobandMesh(fieldMesh, lowThreshold, highThreshold);

	//printf("\n numPolygons: %i ", contourMesh.numPolygons());

	//contourMesh.triangulate();

	//printf("\n tris: %i ", contourMesh.numPolygons());

	/*for (int i = 0; i < contourMesh.numPolygons(); i += 1)
	{
		static vector<int> fVerts;
		contourMesh.getVertices(i, zFaceData, fVerts);

		vector<zVector> points;
		for (int i = 0; i < fVerts.size(); i++)
		{
			points.push_back(contourMesh.vertexPositions[fVerts[i]]);
		}


		zPlane bestPlane = getBestFitPlane(points);

		printf("\n %i , %zi,  %1.2f, %1.2f %1.2f ", i, fVerts.size(), bestPlane.Z.x, bestPlane.Z.y, bestPlane.Z.z);

		int numTris = 0;;;
		vector<int> tris;
		contourMesh.polyTriangulate(i, numTris, tris);


	}*/

	
	//////////////////////////////////////////////////////////


	S = *new SliderGroup();
	
	S.addSlider(&background, "background");

	S.addSlider(&plotOffset, "plotOffset");
	S.sliders[1].attachToVariable(&plotOffset, 0, 50);

	S.addSlider(&requiredGFA, "requiredGFA");
	S.sliders[2].attachToVariable(&requiredGFA, 0, 10000);
	

	S.addSlider(&pBuilding.floorWidth, "f_Width");
	S.sliders[3].attachToVariable(&pBuilding.floorWidth, 0, 30);

	S.addSlider(&floor_numSplits, "f_NumSplits");
	S.sliders[4].attachToVariable(&floor_numSplits, 0, 30);

	
	S.addSlider(&pBuilding.balconyWidth, "b_Width");
	S.sliders[5].attachToVariable(&pBuilding.balconyWidth, 0, 30);

	S.addSlider(&balcony_numSplits, "b_NumSplits");
	S.sliders[6].attachToVariable(&balcony_numSplits, 0, 30);

	S.addSlider(&balcony_numVerts, "b_NV");
	S.sliders[7].attachToVariable(&balcony_numVerts, 0, 30);

	S.addSlider(&balcony_gap_numVerts, "b_GapNV");
	S.sliders[8].attachToVariable(&balcony_gap_numVerts, 0, 30);

	S.addSlider(&balcony_startId, "b_startID");
	S.sliders[9].attachToVariable(&balcony_startId, 0, 100);


	S.addSlider(&pBuilding.terraceWidth, "t_Width");
	S.sliders[10].attachToVariable(&pBuilding.terraceWidth, 0, 30);

	S.addSlider(&terrace_numSplits, "t_NumSplits");
	S.sliders[11].attachToVariable(&terrace_numSplits, 0, 30);

	S.addSlider(&terrace_numVerts, "t_NV");
	S.sliders[12].attachToVariable(&terrace_numVerts, 0, 30);

	S.addSlider(&terrace_gap_numVerts, "t_GapNV");
	S.sliders[13].attachToVariable(&terrace_gap_numVerts, 0, 30);

	S.addSlider(&terrace_startId, "t_startID");
	S.sliders[14].attachToVariable(&terrace_startId, 0, 100);


	S.addSlider(&pBuilding.bridgeWidth, "brg_Width");
	S.sliders[15].attachToVariable(&pBuilding.bridgeWidth, 0, 30);

	S.addSlider(&bridge_num, "brg_Num");
	S.sliders[16].attachToVariable(&bridge_num, 0, 30);

	S.addSlider(&bridge_gap_numVerts, "brg_GV");
	S.sliders[17].attachToVariable(&bridge_gap_numVerts, 0, 30);

	S.addSlider(&bridge_numSplits, "brg_NumSplits");
	S.sliders[18].attachToVariable(&bridge_numSplits, 0, 30);

	S.addSlider(&bridge_startId, "brg_startID");
	S.sliders[19].attachToVariable(&bridge_startId, 0, 100);
	

	S.addSlider(&pBuilding.floorHeight, "floorHeight");
	S.sliders[20].attachToVariable(&pBuilding.floorHeight, 3, 12);
	
	S.addSlider(&pBuilding.maxHeight, "maxHeight");
	S.sliders[21].attachToVariable(&pBuilding.maxHeight, 3, 60);

	S.addSlider(&pBuilding.slabDepth, "slabDepth");
	S.sliders[22].attachToVariable(&pBuilding.slabDepth, 0.2, 1.0);
	

	S.addSlider(&floorId, "floorId");
	S.sliders[23].attachToVariable(&floorId, 0, 100);

	/////////////////////////////

	B = *new ButtonGroup(vec(50, 1000, 0));

	B.addButton(&importPlot, "importPlot");
	B.addButton(&setBOffset, "setBOffset");
	B.addButton(&generateFloor_GFA, "generateFloor_GFA");
	B.addButton(&addFloor, "addFloor");
	B.addButton(&editFloor, "editFloor");

	B.addButton(&dField, "dField");

	B.addButton(&dBuffer, "dBuffer");
	
	

	//////////////////////////////////////////////////////////////////////////


}

void update(int value)
{
	
	// import Plot 
	if (importPlot)
	{
		string fileP = "C:/Users/vishu.b/Desktop/Plot03.obj";		
		importOBJ_HE(pBuilding.plot, fileP);
			
		pBuilding.setBuildingOutline(plotOffset);

		//pBuilding.generateDistanceFieldfromBuilding(floorWidth, balcony_Width, offsetWeight, scalarValues);
		/*double balcony_numVerts = 3.0;
		double balcony_gap_numVerts = 1.0;
		double balcony_numSplits = 1.0;*/	

	
		
			
		importPlot = !importPlot;

		

	}
	
	if (setBOffset)
	{
		if (pBuilding.plot.numPolygons() > 0)
		{
			pBuilding.setBuildingOutline(plotOffset);
		}

		pBuilding.terraceGraph = pBuilding.getTerraceGraph(pBuilding.buildingOutline, offsetWeight, floor(terrace_numSplits), floor(terrace_numVerts), floor(terrace_gap_numVerts), floor(terrace_startId));

		pBuilding.balconyGraph = pBuilding.getBalconyGraph(pBuilding.buildingOutline,  offsetWeight, floor(balcony_numSplits), floor(balcony_numVerts), floor(balcony_gap_numVerts), floor(balcony_startId));

		pBuilding.slabGraph = pBuilding.getSlabGraph(pBuilding.buildingOutline,  offsetWeight, floor(floor_numSplits));

		pBuilding.bridgeGraph = pBuilding.getBridgeGraph(pBuilding.buildingOutline,  offsetWeight, floor(bridge_numSplits), floor(bridge_num), floor(bridge_gap_numVerts), floor(bridge_startId));

		//pBuilding.generateDistanceFieldfromBuilding(floorWidth, balcony_Width, offsetWeight, scalarValues);
		//bufferObj.updateVertexColors(pBuilding.fieldMesh.vertexColors, pBuilding.fieldMesh.VBO_VertexId);
	}

	if (generateFloor_GFA)
	{
		/*pBuilding.generateDistanceFieldfromBuilding(floorWidth, balcony_Width, terrace_Width, offsetWeight, scalarValues);

		pBuilding.generateFloors_GFA(scalarValues, offsetWeight, requiredGFA, maxHeight, floorWidth, floorHeight, slabDepth, terrace_Width, balcony_Width, dBuffer);

		

		if (dBuffer)
		{
			for (int i = 0; i < pBuilding.floors.size(); i++)
			{
				if (floorBuffer.size() <= i)
					floorBuffer.push_back(zBufferObject(100000));
				else floorBuffer[i].clearBufferForRewrite();

				floorBuffer[i].appendMesh(pBuilding.floors[i].floor_slab,true, 80 );
				floorBuffer[i].appendMesh(pBuilding.floors[i].floor_soffit ,true, 80);
				floorBuffer[i].appendMesh(pBuilding.floors[i].glass,true, 80);
			}

			for (int i = pBuilding.floors.size(); i < floorBuffer.size(); i++)
			{
				floorBuffer[i].clearBufferForRewrite();
			}
		}*/

		//generateFloor_GFA = !generateFloor_GFA;
		
	}

	if (addFloor)
	{

		pBuilding.terraceGraph = pBuilding.getTerraceGraph(pBuilding.buildingOutline, offsetWeight, floor(terrace_numSplits), floor(terrace_numVerts), floor(terrace_gap_numVerts), floor(terrace_startId));

		pBuilding.balconyGraph = pBuilding.getBalconyGraph(pBuilding.buildingOutline, offsetWeight, floor(balcony_numSplits), floor(balcony_numVerts), floor(balcony_gap_numVerts), floor(balcony_startId));

		pBuilding.slabGraph = pBuilding.getSlabGraph(pBuilding.buildingOutline, offsetWeight, floor(floor_numSplits));

		pBuilding.bridgeGraph = pBuilding.getBridgeGraph(pBuilding.buildingOutline, offsetWeight, floor(bridge_numSplits), floor(bridge_num), floor(bridge_gap_numVerts), floor(bridge_startId));

		bool floor_fieldBooleans[4]; 
		floor_fieldBooleans[0] = true;  floor_fieldBooleans[1] = false; floor_fieldBooleans[2] = false; floor_fieldBooleans[3] = true;

		pBuilding.addFloor(floor_fieldBooleans, scalarValues, dBuffer);

		//pBuilding.addFloor(scalarValues, offsetWeight, maxHeight, floorWidth, floorHeight, slabDepth, terrace_Width, balcony_Width, dBuffer);

		floorBuffer.push_back(zBufferObject(100000));
		floorBuffer[floorBuffer.size() - 1].appendMesh(pBuilding.floors[pBuilding.floors.size() -1].floor_slab, true, 80);
		floorBuffer[floorBuffer.size() - 1].appendMesh(pBuilding.floors[pBuilding.floors.size() - 1].floor_soffit, true, 80);
		floorBuffer[floorBuffer.size() - 1].appendMesh(pBuilding.floors[pBuilding.floors.size() - 1].glass, true, 80);
		floorBuffer[floorBuffer.size() - 1].appendMesh(pBuilding.floors[pBuilding.floors.size() - 1].floor_balcony, true, 80);

		bufferObj.updateVertexColors(pBuilding.fieldMesh.vertexColors, pBuilding.fieldMesh.VBO_VertexId);

		addFloor = !addFloor;		
	}
		
	
	if (editFloor)
	{
		/*int floorNum = floor(floorId);
		pBuilding.generateDistanceFieldfromBuilding(floorWidth, balcony_Width, terrace_Width, offsetWeight, scalarValues);

		

		pBuilding.editFloor(floorNum,  maxHeight, floorWidth, floorHeight, slabDepth, terrace_Width, balcony_Width, dBuffer);

		for (int i = floorNum ; i < floorBuffer.size(); i++)
		{
			floorBuffer[i].clearBufferForRewrite();

			floorBuffer[i].appendMesh(pBuilding.floors[i].floor_slab, true, 80);
			floorBuffer[i].appendMesh(pBuilding.floors[i].floor_soffit, true, 80);
			floorBuffer[i].appendMesh(pBuilding.floors[i].glass, true, 80);
		}*/

		
	}

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------

void draw()
{

	backGround(background);
	drawGrid(100.0);

	glColor3f(1, 0, 0);
	

	S.draw();
	B.draw();
	// ------------------------ draw the path points / Tool orientations 

	
	drawMesh(pBuilding.plot, true, true, false);
	
	drawMesh(pBuilding.buildingOutline, true, true, false);

	drawGraph(pBuilding.terraceGraph, true, true);
	drawGraph(pBuilding.balconyGraph, true, true);
	drawGraph(pBuilding.slabGraph, true, true);
	drawGraph(pBuilding.bridgeGraph, true, true);
		
	//drawMesh(pBuilding.fieldMesh, false, false, true);

	if(dField)drawQuadsFromBuffer(bufferObj, true);
	
	if (dBuffer)
	{
		for (int i = 0; i < floorBuffer.size(); i++)
		{

			/*glColor3f(0, 0, 0);
			glLineWidth(2.0);
			drawLinesFromBuffer(floorBuffer[i], false);
			glLineWidth(1.0);*/

			drawTrianglesFromBuffer(floorBuffer[i], true);

			

			
		}
	}
	else
	{
		for (int i = 0; i < pBuilding.floors.size(); i++)
		{
			drawMesh(pBuilding.floors[i].floor_slab, false, false, true);

	
			drawMesh(pBuilding.floors[i].floor_soffit, false, false, true);

			drawMesh(pBuilding.floors[i].glass, false, false, true);

			drawMesh(pBuilding.floors[i].floor_balcony, false, false, true);
		}
	}

	

	

	//////////////////////////////////////////////////////////



	glColor3f(0, 0, 0);
	setup2d();

	AL_drawString(s, winW * 0.5, winH - 50);
	AL_drawString(text, winW * 0.5, winH - 75);
	AL_drawString(jts, winW * 0.5, winH - 100);
	AL_drawString(printfile.c_str(), winW * 0.5, winH - 125);
	
	

	int hts = 25;
	int wid = winW * 0.75;


	

	restore3d();
	//drawVector(camPt, vec(wid, hts + 25, 0), "cam");

}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------

void keyPress(unsigned char k, int xm, int ym)
{

	///// GRAPH GENERTOR PROGRAM 
	if (k == 'i')setCamera(15, -40, 60, -2, 4);
	


}

void mousePress(int b, int state, int x, int y)
{

	if (GLUT_LEFT_BUTTON == b && GLUT_DOWN == state)
	{
		
		B.performSelection(x, y);

		S.performSelection(x, y, HUDSelectOn);

	}

	if((GLUT_LEFT_BUTTON == b && GLUT_UP == state) || (GLUT_RIGHT_BUTTON == b && GLUT_UP == state))
	{
		
	}
}

void mouseMotion(int x, int y)
{
	S.performSelection(x, y, HUDSelectOn);
	

	bool dragging = (glutGetModifiers() == GLUT_ACTIVE_ALT) ? true : false;
	int cur_msx = winW * 0.5;
	int cur_msy = winH * 0.5;
	camPt = screenToCamera(cur_msx, cur_msy, 0.2);

	//if( dragging)GS.LM.updateColorArray(lightscale, flipNormals, camPt);

}




#endif // _MAIN_
