
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


////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
/*Objects*/
zObjMesh meshObj;

/*Function sets*/
zFnMesh fnMesh;

/*Tool sets*/
zTsMesh2Pix mesh2Pix;
zUtilsCore coreUtils;

/*General variables*/
string meshPath = "C:/Users/Leo.b/Desktop/funnel.json";
string imgOutputPath = "C:/Users/Leo.b/Desktop/tester";

int trainingSetSize = 40.00;
double translationRange = 0.4;

vector<int> support;


////// --- MODEL / DISPLAY SETUP ----------------------------------------------------
zModel model;

////// --- GUI OBJECTS ----------------------------------------------------
bool display = true;
bool meshToPix = false;
bool computeSupport = true;


double background = 0.2;
double angle = 45.00;

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
	
	// append to model for displaying the object
	model.addObject(meshObj);

	// set display element booleans
	meshObj.setShowElements(true, true, true);
}

void update(int value)
{
	if (computeSupport)
		mesh2Pix.checkVertexSupport(meshObj, angle, support);
	

	if (meshToPix)
	{
		mesh2Pix = zTsMesh2Pix(meshObj);

		string vertexDataFilePath = imgOutputPath + "/meshImage_zVertexVertexData.bmp";
		string connectivityFilePath = imgOutputPath + "/meshImage_zVertexVertex.bmp";

		mesh2Pix.toBMP(zVertexVertex, connectivityFilePath);
		mesh2Pix.toVertexDataBMP(support, vertexDataFilePath);

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