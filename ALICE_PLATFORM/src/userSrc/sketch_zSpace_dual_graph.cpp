//#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v002
// zSpace Library
#include <headers/app/include/zObjects.h>
#include <headers/app/include/zFnSets.h>
#include <headers/app/include/zViewer.h>
// zSpace tool set header
#include <headers/app/include/Tools/zTsStatics.h>

using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// CUSTOM FUNCTIONS ----------------------------------------------------


////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
zModel model;

zObjMesh operateObj;
zFnMesh fnOperateMesh;

zObjGraph dualObj;

string path = "data/TNA_01.obj";

bool create_Dual = false;
bool draw_dualGraph = true;
bool draw_inputMesh = true;

////// --- GUI OBJECTS ----------------------------------------------------
double background = 0.9;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(100000);

	fnOperateMesh = zFnMesh(operateObj);
	fnOperateMesh.from(path, zOBJ);

	// append to model for displaying the object	
	model.addObject(dualObj);
	model.addObject(operateObj);

	// set display element booleans
	operateObj.setShowElements(false, true, true);
	dualObj.setShowElements(false, true);

	S = *new SliderGroup();
	S.addSlider(&background, "background");

	B = *new ButtonGroup(vec(50, 450, 0));
	B.addButton(&draw_inputMesh, "draw_Input");
	B.addButton(&draw_dualGraph, "draw_Dual");
	B.addButton(&create_Dual, "create_Dual");
}

void update(int value)
{
	operateObj.setShowObject(draw_inputMesh);
	dualObj.setShowObject(draw_dualGraph);

	if (create_Dual)
	{
		vector<int> operateEdge_dualEdge;
		vector<int> dualEdge_operateEdge;
		fnOperateMesh.getDualGraph(dualObj, operateEdge_dualEdge, dualEdge_operateEdge, false, true);

		create_Dual = !create_Dual;
	}
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(background);
	//drawGrid(20.0);

	S.draw();
	B.draw();

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

#endif // _MAIN_