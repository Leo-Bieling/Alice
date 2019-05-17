
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
zObjGraph graphObj;
zObjMeshField<double> fieldObj;
zObjGraph contourGraphObj;

zObjGraph unitCenterObj;

/*Function sets*/
zFnGraph fnGraph;
zFnMeshField<double> fnField;
zFnGraph fnUnitCenter;

/*Tool sets*/


string path = "data/hk_graph.json";

zVector fieldBB_min = zVector(-120, -75, 0);
zVector fieldBB_max = zVector(120, 75, 0);
int fieldResolutionX = 100;
int fieldResolutionY = 50;

double unitCentreOffset = 2.25;

vector<double> fieldScalars;
vector<zColor> vertCol;
vector<zVector> graphCentres;
vector<zVector> unitCentres;
vector<int> unitEdgeId;
vector<int> amenityEdgeId;
vector<zVector> perpVec;


////// --- GUI OBJECTS ----------------------------------------------------
double contourThreshold = 0.5;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// construct graph
	fnGraph = zFnGraph(graphObj);
	fnGraph.from(path, zJSON);

	fnUnitCenter = zFnGraph(unitCenterObj);

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
			unitEdgeId.push_back(i);
		// amenityId
		if (fnGraph.getEdgeColor(i) == zColor(1, 0, 0, 1))
			amenityEdgeId.push_back(i);
	}

	// get perpendicular vec
	for (int i = 0; i < fnGraph.numEdges(); i++)
	{
		zVector tmp = fnGraph.getEdgeVector(i);
		tmp = tmp.rotateAboutAxis(zVector(0, 0, 1), 90.0);
		tmp.normalize();
		perpVec.push_back(tmp);	// normalise
	}

	// get graph centers
	fnGraph.getCenters(zEdgeData, graphCentres);

	// split unit graphEdges in half to get unit center edge
	int k = 0;
	for (int i = 0; i < (unitEdgeId.size() / 2); i++)
	{
		fnGraph.splitEdge(unitEdgeId[k]);
		if (k < unitEdgeId.size()) k += 2;
	}

	// get unit centers
	for (int i = 0; i < unitEdgeId.size(); i++)
		unitCentres.push_back(graphCentres[unitEdgeId[i]] + (perpVec[unitEdgeId[i]] * unitCentreOffset));

	// construct new graph
	
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

	S.addSlider(&unitCentreOffset, "unitCentreOffset");
	S.sliders[0].attachToVariable(&unitCentreOffset, 0.0, 10.0);
	S.addSlider(&contourThreshold, "contour");
	S.sliders[1].attachToVariable(&contourThreshold, 0.01, 0.99);

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

	// tmp draw unit centers
	for (int i = 0; i < unitCentres.size(); i++)
		model.displayUtils.drawPoint(unitCentres[i]);
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