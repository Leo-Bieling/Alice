//#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v002
// zSpace header
#include <headers/app/include/zObjects.h>
#include <headers/app/include/zFnSets.h>
#include <headers/app/include/zViewer.h>

// include toolset header
#include <headers/app/include/Tools/zTsGeometry.h>

using namespace zSpace;
using namespace std;


////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
/*model*/
zModel model;

/*Objects*/
zObjMeshField<double> fieldObj;
zObjGraph graphObj;

/*Function sets*/
zFnMeshField<double> fnField;
zFnGraph fnGraph;

/*Tool sets*/

vector<double> scalars;
vector<zVector> scalarPositions;

////// --- GUI OBJECTS ----------------------------------------------------

double threshold = 0.8;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	
	fnField = zFnMeshField<double>(fieldObj);

	fnField.create(zVector(-20, -20, 0), zVector(20, 20, 0), 150, 150, 1, true, false);  // _N_X && _N_Y > 70 NOT WORKING

	fnField.setFieldColorDomain(zDomainColor(zColor(0, 1, 1), zColor(180, 1, 1)));
	
	// Circle
	//fnField.getScalars_Circle(scalars, zVector(0, 0, 0), 1.0);

	// Line
	//fnField.getScalars_Line(scalars, zVector(-10, 10, 0), zVector(10, -10, 0));

	// Square
	//fnField.getScalars_Square(scalars, zVector(5,5,0));

	// Trapezoid
	fnField.getScalars_Trapezoid(scalars, 2.0, 6.0, 10.0);

	

	fnField.setFieldValues(scalars);


	////// --- MODEL / DISPLAY SETUP ----------------------------------------------------
	model = zModel(100000);
	model.setShowBufQuads(true, true);
	//model.setShowBufLines(true);
	model.addObject(graphObj);
	model.addObject(fieldObj);
	fieldObj.appendToBuffer();

	////// --- GUI SETUP ----------------------------------------------------
	S = *new SliderGroup(vec(50, 50, 0));
	S.addSlider(&threshold, "threshold");
	S.sliders[0].attachToVariable(&threshold, 0.0, 1.0);
}

void update(int value)
{
	fnField.getIsocontour(graphObj, threshold);
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	drawGrid(20);
	backGround(0.75);
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