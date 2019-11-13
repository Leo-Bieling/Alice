//#define _MAIN_

#ifdef _MAIN_

// alice library
#include "main.h"

// zSpace v002
// zSpace Library
#include <headers/app/include/zObjects.h>
#include <headers/app/include/zFnSets.h>
#include <headers/app/include/zViewer.h>

// zSpace toolset header
#include <headers/app/include/Tools/zTsStatics.h>


using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// CUSTOM METHODS ----------------------------------------------------
void drawTextAtVec(string s, zVector &pt)
{
	unsigned int i;
	glRasterPos3f(pt.x, pt.y, pt.z);

	for (i = 0; i < s.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
}

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------

zModel model;

zObjMesh operateObj;
zFnMesh fnOperateMesh;

zObjGraph dualObj;
zFnGraph fnDual;

zObjMesh forceObj;
zObjMesh formObj;
zObjMesh resultObj;

zTsMeshVault myVault;

vector<zVector> formtargets;
bool gettargets = true;

vector<int> fixedVertices;

double formWeight = 1.0;
double angleTolerance = 0.001;
double forceTolerance = 0.001;

double form_minMax = 0.05;
double force_minMax = 0.01;
double forceDiagramScale = 2;


double vMass = 5.0;
double vThickness = 0.1;

double dT = 0.5;
zIntergrationType intType = zRK4;

vector<zVector> forces;

string path = "C:/Users/vishu.b/Desktop/TNA_01.obj";

bool fixBoundary = false;

bool splitDual = false;
bool c_form = false;
bool c_force = false;



bool horizontalE= false;
bool verticalE = false;

bool c_Result = false;

bool d_dualgraph = true;
bool d_inputmesh = true;
bool d_ForceDiagram = true;
bool d_FormDiagram = true;
bool d_Result = true;

bool rotate90 = true;
bool computeHE_targets = true;
bool computeFDs = true;

bool FDM = false;

bool walkNext = false;

int currentId = 0;
int rCnt = 0;
////// --- GUI OBJECTS ----------------------------------------------------


char s[200],text[200], text1[200], jts[400];

double background = 0.9;
vec camPt;
string printfile;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------




////// ---------------------------------------------------- MODEL  ----------------------------------------------------





void setup()
{

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	//////////////////////////////////////////////////////////////////////////
	model = zModel(100000);
	   	 
	myVault = zTsMeshVault(resultObj, formObj, forceObj);

	fnOperateMesh = zFnMesh(operateObj);

	fnOperateMesh.from("", zJSON);

	// append to model for displaying the object	
	model.addObject(resultObj);
	model.addObject(formObj);
	model.addObject(forceObj);

	// set display element booleans

	

	// buffer
	///operateObj.appendMeshToBuffer();

	
	//////////////////////////////////////////////////////////


	S = *new SliderGroup();
	
	S.addSlider(&background, "background");

	



	S.addSlider(&formWeight, "formWeight");
	S.sliders[1].attachToVariable(&formWeight, 0.00, 1.00);

	S.addSlider(&form_minMax, "form_minMax");
	S.sliders[2].attachToVariable(&form_minMax, 0.0001, 1.00);

	S.addSlider(&force_minMax, "force_minMax");
	S.sliders[3].attachToVariable(&force_minMax, 0.0001, 1.00);

	S.addSlider(&angleTolerance, "angleTolerance");
	S.sliders[4].attachToVariable(&angleTolerance, 0.001, 10.00);

	S.addSlider(&vMass, "vMass");
	S.sliders[5].attachToVariable(&vMass, 0.01, 10.00);

	S.addSlider(&vThickness, "vThickness");
	S.sliders[6].attachToVariable(&vThickness, 0.01, 1.0);

	S.addSlider(&forceDiagramScale, "forceScale");
	S.sliders[7].attachToVariable(&forceDiagramScale, 0.01, 10.00);
	
	S.addSlider(&forceTolerance, "forceTolerance");
	S.sliders[8].attachToVariable(&forceTolerance, 0.001, 1.00);


	/////////////////////////////

	B = *new ButtonGroup(vec(50, 450, 0));

	B.addButton(&c_Result, "c_Result");

	B.addButton(&c_form, "c_form");

	B.addButton(&c_force, "c_force");

	B.addButton(&horizontalE, "horizontalE");	

	B.addButton(&verticalE, "verticalE");	

	B.addButton(&d_ForceDiagram, "d_ForceDiagram");

	B.addButton(&d_FormDiagram, "d_FormDiagram");

	B.addButton(&d_Result, "d_Result");

	B.addButton(&walkNext, "walkNext");
	

	//////////////////////////////////////////////////////////////////////////


}

void update(int value)
{
	operateObj.setShowObject(d_inputmesh);
	formObj.setShowObject(d_FormDiagram);
	forceObj.setShowObject(d_ForceDiagram);
	resultObj.setShowObject(d_Result);

	if (c_Result)
	{
		myVault.createResultfromFile(path, zOBJ);
		
		
		
		//vector<int> fixedVerts = { 2,3,4,5,8,9,10,11,12,13,15,16,19,21,26,77,78,81,82,91,92,95,96,97,98,101,102,103,104 };
		//vector<int> fixedVerts1 = { 8,9,10,11,13,15,97,98,101,102 };
		vector<int> fixedVerts1 = { 30,31,62,63,64,95,96,127,128,159};
		myVault.setConstraints(zResultDiagram/*, fixedVerts1*/);
		

		// set vertexWeights 
		myVault.setVertexWeightsfromConstraints(zDiagramType::zResultDiagram);

		// set default tension
	
		vector<int> fixedVerts = { 0,1,6,7,8,9,10,11,13,14,15,17,23,25,27,73,74,85,86,89,90,93,94,97,98,99,100,101,102 };
		//vector<int> fixedVerts = { 2,3,4,5,8,9,10,11,12,13,15,16,19,21,26,77,78,81,82,91,92,95,96,97,98,101,102,103,104 };
		
		vector<bool> fixVerts_bool;
		for (int i = 0; i < myVault.fnResult.numVertices(); i++) fixVerts_bool.push_back(false);
		for (int i = 0; i < fixedVerts.size(); i++)fixVerts_bool[fixedVerts[i]] = true;

		vector<int> tensionEdges;

		for (int i = 0; i < myVault.fnResult.numEdges(); i += 2)
		{
			if (myVault.fnResult.onBoundary(i, zEdgeData) || myVault.fnResult.onBoundary(i + 1, zEdgeData))
			{
				int v0 = myVault.fnResult.getEndVertexIndex(i);
				int v1 = myVault.fnResult.getStartVertexIndex(i);

				if (!fixVerts_bool[v0] || !fixVerts_bool[v1])
				{
					tensionEdges.push_back(i);
					tensionEdges.push_back(i + 1);

					if (myVault.fnResult.onBoundary(i, zEdgeData))currentId = i;
					else currentId = i + 1;

				}
			}


		}
		myVault.setTensionEdges(zResultDiagram/*, tensionEdges*/);

		// set vertex mass
		myVault.setVertexMass(vMass);

		//myVault.setVertexMassfromVertexArea();

		// set vertex thickness
		myVault.setVertexThickness(vThickness);

		resultObj.setShowElements(false, true, true);

		c_Result = !c_Result;
	}
	
	if (c_form)
	{
		
		myVault.createFormFromResult();

		myVault.setVertexWeights(zFormDiagram);

		formObj.setShowElements(true, true, false);

		computeHE_targets = true;

		computeFDs = true;

		c_form = !c_form;

	}

	if (c_force)
	{
		myVault.createForceFromForm(rotate90);

		myVault.translateForceDiagram(2.0);

		forceObj.setShowElements(true, true, false);
		
		c_force = !c_force;
	}

	if (horizontalE)
	{
		bool out = myVault.equilibriumHorizontal(computeHE_targets, formWeight, dT, intType,1, angleTolerance, form_minMax, force_minMax, true, true);

		computeFDs = true;

		horizontalE = !out;


	}




	if (verticalE)
	{
		// set vertex mass
		//myVault.setVertexMass(vMass);

		myVault.setVertexMassfromVertexArea();

		// set vertex thickness
		myVault.setVertexThickness(vThickness);

		//bool out  =myVault.verticalEquilibrium(dT, intType, 1.0, forceTolerance);

		bool out = myVault.equilibriumVertical(computeFDs,forceDiagramScale);

		myVault.getForces(forces);

		verticalE = !out;

	}

	// walk
	if (walkNext)
	{
		
		
		int forceEdge = myVault.getCorrespondingForceEdge(currentId);

		myVault.fnForm.setEdgeColor(currentId, zColor());
		myVault.fnForm.setEdgeWeight(currentId, 1.0);
		vector<int> eVerts;
		myVault.fnForm.getVertices(currentId, zEdgeData, eVerts);
		myVault.fnForm.setVertexColor(eVerts[0], zColor(1, 0, 0, 1));
		myVault.fnForm.setVertexColor(eVerts[1], zColor(1, 0, 0, 1));

		myVault.fnForce.setEdgeColor(forceEdge, zColor());
		myVault.fnForce.setEdgeWeight(forceEdge, 1.0);
		myVault.fnForce.getVertices(forceEdge, zEdgeData, eVerts);
		myVault.fnForce.setVertexColor(eVerts[0], zColor(1, 0, 0, 1));
		myVault.fnForce.setVertexColor(eVerts[1], zColor(1, 0, 0, 1));

		currentId = myVault.fnForm.getPrevIndex(currentId); 

		forceEdge = myVault.getCorrespondingForceEdge(currentId);

		if (forceEdge != -1)
		{
			myVault.fnForm.setEdgeColor(currentId, zColor(1, 0, 0, 1));
			myVault.fnForm.setEdgeWeight(currentId, 3.0);
			vector<int> eVerts;
			eVerts.clear();
			myVault.fnForm.getVertices(currentId, zEdgeData, eVerts);
			myVault.fnForm.setVertexColor(eVerts[0], zColor(0, 0, 1, 1));
			myVault.fnForm.setVertexColor(eVerts[1], zColor(0, 1, 0, 1));

			myVault.fnForce.setEdgeColor(forceEdge, zColor(1, 0, 0, 1));
			myVault.fnForce.setEdgeWeight(forceEdge, 3.0);
			myVault.fnForce.getVertices(forceEdge, zEdgeData, eVerts);
			myVault.fnForce.setVertexColor(eVerts[0], zColor(0, 0, 1, 1));
			myVault.fnForce.setVertexColor(eVerts[1], zColor(0, 1, 0, 1));

		}

		walkNext = !walkNext;
	}

}


////// ---------------------------------------------------- VIEW  ----------------------------------------------------

void draw()
{

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	backGround(background);
	//drawGrid(20.0);

	glColor3f(1, 0, 0);
	

	S.draw();
	B.draw();
	// ------------------------ draw the path points / Tool orientations 

	model.draw();	
	
	if (forces.size() > 0)
	{
		zVector *pos = myVault.fnResult.getRawVertexPositions();
		for (int i = 0; i < forces.size(); i++)
		{
			zVector p1 = forces[i];
			p1.normalize(); 
			p1* 0.1;

			model.displayUtils.drawLine(pos[i], pos[i] + p1);
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
