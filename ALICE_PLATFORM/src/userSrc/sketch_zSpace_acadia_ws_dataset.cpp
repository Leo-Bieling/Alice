
//#define _MAIN_

#ifdef _MAIN_

#include <chrono>

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

		if (vIt.getPosition().z > 0)
			(ang > (angle_threshold)) ? support[vIt.getId()] = 2 : support[vIt.getId()] = 1;
		else
			support[vIt.getId()] = 1;
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
string meshPath = "C:/Users/Leo.b/Desktop/open_cube.json";
//string imgOutputPath = "C:/Users/Leo.b/Desktop/dataset";
string imgOutputPath = "//zaha-hadid.com/Data/Projects/1453_CODE/1453___research/res_leo/research/workshops/ACADIA_19/dataSet_v0";

vector<int> support;
int trainingSetSize = 100;
double angle = 10.00;
float translationRange = 0.4;


////// --- GUI OBJECTS ----------------------------------------------------
double background = 0.2;

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	fnMesh = zFnMesh(meshObj);

	auto t1 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < trainingSetSize; i++)
	{
		fnMesh.from(meshPath, zJSON);

		// translate vertices
		zPointArray vertPos;
		fnMesh.getVertexPositions(vertPos);

		for (int i = 0; i < vertPos.size(); i++)
			vertPos[i] += zVector(coreUtils.randomNumber_double(translationRange * -1, translationRange), coreUtils.randomNumber_double(translationRange * -1, translationRange), 0);

		fnMesh.setVertexPositions(vertPos);

		// smooth
		//fnMesh.smoothMesh(1);

		// check suuport
		checkVertexSupport(meshObj, angle, support);
		
		// create BMPs
		meshImage = zTsMesh2Pix(meshObj);
		meshImage.toBMP(imgOutputPath, zVertexVertex, i);
		meshImage.toBMP(support, imgOutputPath, i);

		// write obj
		string objOutput = imgOutputPath + "/mesh_" + to_string(i) + ".obj";
		fnMesh.to(objOutput, zOBJ);
	}
	
	auto t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();


	std::cout << "\n" << duration / 1000 << " seconds";
}

void update(int value)
{
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	drawGrid(50);
	backGround(background);
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