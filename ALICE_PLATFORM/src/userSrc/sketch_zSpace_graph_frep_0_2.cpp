
#define _MAIN_

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
zObjGraph graphObj;
zObjMeshField<double> fieldObj;
zObjGraph contourGraphObj;


/*Function sets*/
zFnGraph fnGraph;
zFnMeshField<double> fnField;

/*Tool sets*/


string path = "data/hk_graph_v2.json";

zVector fieldBB_min = zVector(-120, -75, 0);
zVector fieldBB_max = zVector(120, 75, 0);
int fieldResolutionX = 100;
int fieldResolutionY = 50;


vector<double> fieldScalars;
vector<zColor> vertCol;

vector<int> greenEdgeId;
vector<int> redEdgeId;

vector<int> greenVertIds;
vector<int> greenVertsVal1Id;
vector<zVector> unitCentres;


////// --- GUI OBJECTS ----------------------------------------------------
double contourThreshold = 0.5;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// construct graph
	fnGraph = zFnGraph(graphObj);
	fnGraph.from(path, zJSON);


	// construct field
	fnField = zFnMeshField<double>(fieldObj);
	fnField.create(fieldBB_min, fieldBB_max, fieldResolutionX, fieldResolutionY, 1, true, false);

	// compute scalars
	fnField.getScalarsAsVertexDistance(fieldScalars, graphObj, 10, 200);

	// set color domain of field
	fnField.setFieldColorDomain(zDomainColor(zColor(0, 1, 1), zColor(240, 1, 1)));
	
	// set field values
	fnField.setFieldValues(fieldScalars);


	// get unit and amentity edge id
	for (int i = 0; i < fnGraph.numEdges(); i++)
	{
		// unitId
		if (fnGraph.getEdgeColor(i) == zColor(0, 1, 0, 1))
			greenEdgeId.push_back(i);
		// amenityId
		if (fnGraph.getEdgeColor(i) == zColor(1, 0, 0, 1))
			redEdgeId.push_back(i);
	}

	// get valence 1 of green edges
	for (int i = 0; i < greenEdgeId.size(); i++)
	{
		greenVertIds.push_back(fnGraph.getStartVertexIndex(greenEdgeId[i]));
		greenVertIds.push_back(fnGraph.getEndVertexIndex(greenEdgeId[i]));
	}

	for (int i = 0; i < greenVertIds.size(); i++)
	{
		if (fnGraph.checkVertexValency(greenVertIds[i]))
		{
			greenVertsVal1Id.push_back(greenVertIds[i]);
			unitCentres.push_back(fnGraph.getVertexPosition(greenVertIds[i]));
		}
	}

	// get frep box at origin

	// translate to center of unit

	////// --- MODEL / DISPLAY SETUP ----------------------------------------------------
	model = zModel(10000);
	graphObj.setShowElements(true, true);
	//model.setShowBufQuads(true, true);
	model.addObject(graphObj);
	model.addObject(contourGraphObj);
	//model.addObject(fieldObj);
	//fieldObj.appendToBuffer();

	////// --- GUI OBJECTS ----------------------------------------------------

	S = *new SliderGroup(vec(50, 50, 0));

	S.addSlider(&contourThreshold, "contour");
	S.sliders[0].attachToVariable(&contourThreshold, 0.01, 0.99);

}

void update(int value)
{
	// get contour graph
	fnField.getIsocontour(contourGraphObj, contourThreshold);

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	//drawGrid(20);
	backGround(0.75);
	model.draw();

	for each (zVector p in unitCentres)
		model.displayUtils.drawPoint(p, zColor(0, 0, 1, 1), 10.0);	
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