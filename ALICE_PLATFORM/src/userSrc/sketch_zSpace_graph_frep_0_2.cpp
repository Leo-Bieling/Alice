
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


/*Function sets*/
zFnGraph fnGraph;
zFnMeshField<double> fnField;


/*Tool sets*/

string path = "data/hk_graph_v2.json";

zVector fieldBB_min = zVector(-120, -75, 0);
zVector fieldBB_max = zVector(120, 75, 0);
int fieldResolutionX = 200;
int fieldResolutionY = 100;


vector<double> fieldScalars;

vector<int> greenEdgeIds;
vector<int> redEdgeIds;

vector<int> greenVertIds;
vector<int> greenVal1VertsId;

vector<zVector> unitCentres;

vector<zTransformationMatrix> unitTransfMats;
vector<zTransform> transMats;

vector<zObjGraph> graphTestObj;
vector <zFnGraph> fnGraphTest;
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

	// get unit and amenity edge id
	for (int i = 0; i < fnGraph.numEdges(); i++)
	{
		// unitId
		if (fnGraph.getEdgeColor(i) == zColor(0, 1, 0, 1))
			greenEdgeIds.push_back(i);
		// amenityId
		if (fnGraph.getEdgeColor(i) == zColor(1, 0, 0, 1))
			redEdgeIds.push_back(i);
	}

	printf("\n\ngreenEdgeIds count: %i", greenEdgeIds.size());
	printf("\nredEdgeIds count: %i", redEdgeIds.size());

	// get valence 1 vertices of green edges
	for (int i = 0; i < greenEdgeIds.size(); i++)
	{	
		//// get start and end vertex of each edge
		greenVertIds.push_back(fnGraph.getStartVertexIndex(greenEdgeIds[i]));
		greenVertIds.push_back(fnGraph.getEndVertexIndex(greenEdgeIds[i]));
	}

	for (int i = 0; i < greenVertIds.size(); i++)
	{	
		if (fnGraph.checkVertexValency(greenVertIds[i]))
		{
			greenVal1VertsId.push_back(greenVertIds[i]);
			unitCentres.push_back(fnGraph.getVertexPosition(greenVertIds[i]));
		}
	}

	printf("\nunitCentres count: %i", unitCentres.size());

	// get rotation
	vector<float> zRotation;
	for (int i = 0; i < greenVal1VertsId.size(); i++)
	{
		vector<int> tmpEdgeIds;
		fnGraph.getConnectedEdges(greenVal1VertsId[i], zVertexData, tmpEdgeIds);

		//// compute rotation
		zVector tmp = fnGraph.getEdgeVector(tmpEdgeIds[0]);
		zRotation.push_back(tmp.angle(zVector(1, 0, 0)));

		tmpEdgeIds.clear();

		//cout << zRotation[i] << endl;
	}

	

	// get frep box at origin

	// translate to center of unit




	for (int i = 0; i < unitCentres.size(); i++)
	{
		graphTestObj.push_back(zObjGraph());
		fnGraphTest.push_back(zFnGraph(graphTestObj[i]));
		fnGraphTest[i].from("C:/Users/Leo.b/Desktop/tester.json", zJSON);

		// rotation
		double3 r = { 0, 0, zRotation[0] };
		fnGraphTest[i].setRotation(r);

		// translation
		fnGraphTest[i].setTranslation(unitCentres[0]);



		//testGraphsObj.push_back(graphTestObj);
	}




	////// --- MODEL / DISPLAY SETUP ----------------------------------------------------
	model = zModel(100000);

	for (int i = 0; i < graphTestObj.size(); i++)
	{
		graphTestObj[i].setShowElements(true, true);
		model.addObject(graphTestObj[i]);
	}


	//graphObj.setShowElements(true, true);
	//model.setShowBufQuads(true, true);

	//model.addObject(graphObj);
	//model.addObject(contourGraphObj);

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

	//for each (zVector p in unitCentres)
	//	model.displayUtils.drawPoint(p, zColor(0, 0, 1, 1), 10);

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