//#define _MAIN_


#ifdef _MAIN_

#include "main.h"

//////  zSpace Library
//#include <zSpace/zSpaceImplement.h>
//#include <zSpace/zDatabase.h>
//#include <zSpace/zOpenStreet.h>
//#include <zSpace/zDisplay.h>


#include <headers/include/zCore.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zData.h>
#include <headers/include/zIO.h>



using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------

double increment = 1;

zBufferObject bufferObj;

zMesh myMesh;
zGraph myGraph;


bool tris = false;
bool split = false;
bool deleteItem = false;
bool fNorms = false;

int rCnt = 0;
////// --- GUI OBJECTS ----------------------------------------------------


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

	


	fromOBJ(myMesh, "C:/Users/leo.b/Desktop/zSpace_test.obj");

	fromMESH(myGraph, myMesh);

	//toTXT(myGraph, "C:/Users/vishu.b/Desktop/zSpace_test.txt");

	//fromTXT(myGraph, "C:/Users/vishu.b/Desktop/zSpace_graph.txt");

	//triangulate(myMesh);

	myMesh.vertexColors[29] = zColor(0, 0, 1, 1);

	bufferObj.appendMesh(myMesh);
	//toOBJ(myMesh, "C:/Users/vishu.b/Desktop/zSpace_test_1.obj");

	//myMesh.vertexColors[0] = zColor(0, 0, 1, 0);
	

	vector<float> dists;
	vector<int> parent;
	vector<int> edgepath;

	
		
	/*shortestPath(myGraph, 0, 8, edgepath);
	printf("\n edgepath : ");
	for (int i = 0; i < edgepath.size(); i++)
	{
		printf(" %i ", edgepath[i]);

		myGraph.edgeColors[edgepath[i]] = zColor(1, 0, 0, 1);
		myGraph.edgeWeights[edgepath[i]] = 10;

		(edgepath[i]%2 == 0) ? myGraph.edgeColors[edgepath[i] +1] = zColor(1, 0, 0, 1) : myGraph.edgeColors[edgepath[i]-1] = zColor(1, 0, 0, 1);
		(edgepath[i] % 2 == 0) ? myGraph.edgeWeights[edgepath[i] + 1] = 10 : myGraph.edgeWeights[edgepath[i] - 1] = 10;
	}*/


	vector<int> edgeVisited;
	shortestPathWalks(myGraph, edgeVisited);

	//fromCSV("C:/Users/vishu.b/Desktop/zSpace_graph.csv", zEdgeData, myGraph, edgeVisited);

	int minVisits = zMin(edgeVisited);
	int maxVisits = zMax(edgeVisited);

	printf("\n minVisits: %i , maxVisits: %i ", minVisits, maxVisits);

	for (int i = 0; i < myGraph.numEdges(); i++)
	{
		double weight = ofMap((double)edgeVisited[i], (double)minVisits, (double)maxVisits, (double)1.0, (double)10.0);

		myGraph.edgeWeights[i] = weight;

	
	}


	//toCSV("C:/Users/vishu.b/Desktop/zSpace_graph.csv", zEdgeData, myGraph, edgeVisited);
	
	
	//////////////////////////////////////////////////////////


	S = *new SliderGroup();
	
	S.addSlider(&background, "background");


	//S.addSlider(&lowThreshold, "thresholdLow");
	//S.sliders[1].attachToVariable(&lowThreshold, 0, 1);

	

	/////////////////////////////

	B = *new ButtonGroup(vec(50, 450, 0));

	B.addButton(&tris, "tris");

	B.addButton(&split, "split");

	B.addButton(&deleteItem, "deleteItem");
	
	B.addButton(&fNorms, "fNorms");


	

	//////////////////////////////////////////////////////////////////////////


}

void update(int value)
{
	if (tris)
	{

		

		triangulate(myMesh);

		printMesh(myMesh);


		tris = !tris;
	}

	if (split)
	{
		
		//--- subdivide mesh
		subDivideMesh(myMesh, 1);

		//--- split Edge 
		/*int numOriginaledges = myMesh.numEdges();

		for (int i = 0; i < numOriginaledges; i += 2)
		{			
			splitEdge(myMesh, i);
		}*/
		

		//--- split face
		//vector<int> edgeList = { 2 , 8, 6 };
		//vector<double> edgeFactor = { 0.2 ,0.8, 0.8};
		//splitFaces(myMesh, edgeList, edgeFactor);

		printMesh(myMesh);


		split = !split;
	}

	if (fNorms)
	{

		printf("\n numPolygons %i  ", myMesh.numPolygons());

		for (int i= 0; i < myMesh.numPolygons(); i++)
		{
			vector<int> fEdges;
			myMesh.getEdges(i, zFaceData, fEdges);

			printf("\n %i : ", i);

			for (int j = 0; j < fEdges.size(); j++)
			{
				printf(" %i ", fEdges[j]);
			}
		}

		myMesh.computeMeshNormals();

		fNorms = !fNorms;
	}

	if (deleteItem)
	{

		printMesh(myMesh);

		//vector<int> vertexList = {28 };
		deleteVertex(myMesh, 29);

		printMesh(myMesh);

	

		deleteItem = !deleteItem;
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

	//drawMesh(myMesh, true, true, false);
	drawGraph(myGraph, true, true);

	
	
	//vector<zVector> fCenters;
	//getCenters(myMesh, zFaceData, fCenters);
	//drawMesh_FaceNormals(myMesh, fCenters, 0.5);
	
	//drawGraph(openStreet.roadGraph, false, true);
	

	//drawQuadsFromBuffer(bufferObj, true);
	
	//printf("\n nP: %i", myMesh.numPolygons());

	
	

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
