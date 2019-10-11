
#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v003
// zSpace header
#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>

// include toolset header
#include <headers/zApp/include/zTsGeometry.h>

using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// Custom Method

void checkVertexSupport(zObjMesh& _objMesh, double angle_threshold, vector<int>& support)
{
	support.assign(_objMesh.mesh.n_v, -1);

	zFnMesh fnMesh(_objMesh);

	zVector* positions = fnMesh.getRawVertexPositions();

	for (zItMeshVertex vIt(_objMesh); !vIt.end(); vIt++)
	{
		zIntArray cVerts;
		vIt.getConnectedVertices(cVerts);

		int lowestId;
		double val = 10e10;
		for (int i = 0; i < cVerts.size(); i++)
		{
			double zVal = positions[cVerts[i]].z;

			if (zVal < val)
			{
				lowestId = cVerts[i];
				val = zVal;
			}
		}

		zVector lowestV = positions[lowestId];

		zVector vec = vIt.getPosition() - lowestV;
		zVector unitz = zVector(0, 0, 1);

		double ang = vec.angle(unitz);

		(ang > (180 - angle_threshold)) ? support[vIt.getId()] = 2 : support[vIt.getId()] = 1;

	}
}

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
/*Objects*/
zObjMesh meshObj;

/*Function sets*/
zFnMesh fnMesh;

/*Tool sets*/
zTsMesh2Pix meshImage;
zUtilsCore coreUtils;

/*General variables*/
string meshPath = "C:/Users/Leo.b/Desktop/funnel.json";
string imgOutputPath = "C:/Users/Leo.b/Desktop";

int trainingSetSize = 25;
double translationRange = 0.4;

vector<int> support;


////// --- MODEL / DISPLAY SETUP ----------------------------------------------------
zModel model;

////// --- GUI OBJECTS ----------------------------------------------------
bool display = true;
bool meshToPix = false;
bool computeSupport = true;


double background = 0.75;
double angle = 90.00;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	////////////////////////////////////////////////////////////////////////// Sliders

	S = *new SliderGroup();

	S.addSlider(&background, "background");
	S.sliders[0].attachToVariable(&background, 0, 1);

	S.addSlider(&angle, "overhang");
	S.sliders[1].attachToVariable(&angle, 0, 180);

	////////////////////////////////////////////////////////////////////////// Buttons

	B = *new ButtonGroup(Alice::vec(50, 450, 0));

	B.addButton(&computeSupport, "computeSupport");
	B.buttons[0].attachToVariable(&computeSupport);

	B.addButton(&display, "displaySupport");
	B.buttons[1].attachToVariable(&display);

	B.addButton(&meshToPix, "meshToPix");
	B.buttons[2].attachToVariable(&meshToPix);

	////////////////////////////////////////////////////////////////////////// Initialize Model / Objects

	model = zModel(10000);
	model.setShowBufPoints(true, true);

	fnMesh = zFnMesh(meshObj);
	fnMesh.from(meshPath, zJSON);
	
	fnMesh.setFaceColor(zColor(0.75, 0.75, 0.75, 1), true);


	//// translate vertices to create training set
	//zPointArray vertPos;
	//fnMesh.getVertexPositions(vertPos);

	//for (int i = 0; i < vertPos.size(); i++)
	//	vertPos[i] += zVector(coreUtils.randomNumber_double(translationRange * -1, translationRange), coreUtils.randomNumber_double(translationRange * -1, translationRange), coreUtils.randomNumber_double(translationRange * -1, translationRange));

	//fnMesh.setVertexPositions(vertPos);

	//fnMesh.smoothMesh(1);


	// append to model for displaying the object
	model.addObject(meshObj);

	// set display element booleans
	meshObj.setShowElements(true, true, true);

	////// --- GUI SETUP ----------------------------------------------------

}

void update(int value)
{
	if (computeSupport)
	{
		checkVertexSupport(meshObj, angle, support);

		int id = 0;
		model.displayUtils.bufferObj.updateVertexColors(fnMesh.getRawVertexColors(), fnMesh.numVertices(), id);

		//run = !run;
	}

	if (meshToPix)
	{
		// create BMPs
		meshImage = zTsMesh2Pix(meshObj);
		meshImage.toBMP(imgOutputPath, zVertexVertex);
		meshImage.toBMP(imgOutputPath, support);

		cout << "\nmeshToPix: success!";

		meshToPix = !meshToPix;
	}
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	drawGrid(50);
	backGround(background);
	model.draw();

	S.draw();
	B.draw();

	if (display)
	{
		for (zItMeshVertex vIt(meshObj); !vIt.end(); vIt++)
		{
			int check = support[vIt.getId()];

			if (check == 1)
				model.displayUtils.drawPoint(vIt.getPosition(), zColor(0, 1, 0, 1), 5);

			if (check == 2)
			{
				double d = vIt.getPosition() * zVector(0, 0, 1);
				zVector proj = zVector(vIt.getPosition().x, vIt.getPosition().y, vIt.getPosition().z - d);

				glEnable(GL_LINE_STIPPLE);
				glLineStipple(1, 0x0101);

				model.displayUtils.drawPoint(vIt.getPosition(), zColor(1, 0, 0, 1), 5);
				model.displayUtils.drawLine(vIt.getPosition(), proj, zColor(1, 0, 0, 1), 1);

				glDisable(GL_LINE_STIPPLE);
			}
		}
	}
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