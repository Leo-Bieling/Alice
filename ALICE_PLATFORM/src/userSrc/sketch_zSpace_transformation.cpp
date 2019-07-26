
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
/*Objects*/

/*Function sets*/

/*Tool sets*/

//zObjPointCloud pointsObj;
//zFnPointCloud fnPoints;

//zObjMesh meshObj;
//zFnMesh fnMesh;

zVector pos(0, 0, 0);
zVector newPos;

zTransformationMatrix transMatrix;
zVector translation ( 20, 20, 0 );

////// --- MODEL / DISPLAY SETUP ----------------------------------------------------
zModel model;

////// --- GUI OBJECTS ----------------------------------------------------


////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(10000);
		
	//fnPoints = zFnPointCloud(pointsObj);

	//fnPoints.addPosition(zVector(0, 0, 0));

	//fnPoints.setTranslation(translation);

	/*fnMesh = zFnMesh(meshObj);
	fnMesh.from("data/app_polytopalmesh_0.obj", zOBJ);*/
	

	//zVector p = fnMesh.getVertexPosition(0);
	//fnMesh.setPivot(p);

	//double3 scale = { 1,1,0.1 };
	//fnMesh.setScale(scale);

	//double3 rot = { 0,0,45 };
	//fnMesh.setRotation(rot);

	//transMatrix(0, 3) = 20;
	//transMatrix(1, 3) = 20;

	double3 t;
	translation.getComponents(t);
	
	// get pivot translation and inverse pivot translations
	zTransform pivotTransMat = transMatrix.asPivotTranslationMatrix();
	zTransform invPivotTransMat = transMatrix.asInversePivotTranslationMatrix();

	// get plane to plane transformation
	zTransformationMatrix to = transMatrix;
	to.setTranslation(t, false);
	zTransform toMat = transMatrix.getToMatrix(to);

	// compute total transformation
	zTransform transMat = invPivotTransMat * toMat * pivotTransMat;

	// transform object
	newPos = pos * transMat;

	

	

	cout << transMat << endl;

	//pointsObj.setShowTransform(true);
	//pointsObj.setShowElements(true);
	//model.addObject(pointsObj);

	////// --- GUI SETUP ----------------------------------------------------
}

void update(int value)
{

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	drawGrid(20);
	backGround(0.75);
	model.draw();

	//model.displayUtils.drawPoint(pos, zColor(1, 0, 0, 1), 20);

	//model.displayUtils.drawPoint(newPos, zColor(0, 0, 1, 1), 20);

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