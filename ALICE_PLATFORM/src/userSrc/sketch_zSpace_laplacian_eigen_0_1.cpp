
//#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v002
// zSpace header
#include <headers/app/include/zObjects.h>
#include <headers/app/include/zFnSets.h>
#include <headers/app/include/zViewer.h>

// zSpace toolset header
#include <headers/app/include/Tools/zTsGeometry.h>

using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
/*model*/
zModel model;

/*Objects*/
vector<zObjMesh> operateMeshObjs;
zObjBuffer myBuffer;

/*Function sets*/
vector<zFnMesh> fnOperateMeshes;

/*Tool sets*/
vector<zTsSpectral> mySpectrals;

// directory of files
string path = "C:/Users/Leo.b/Desktop/facade_meshes";
vector<string> meshFiles;

double eigenFrequency = 1;

////// --- GUI OBJECTS ----------------------------------------------------

bool computeEigen = false;

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// set up model
	model = zModel(1000000);
	model.setShowBufQuads(true, true);
	model.setShowBufLines(true, false);

	// get file directories
	model.coreUtils.getFilesFromDirectory(meshFiles, path, zJSON);
	printf("\n numberOfFiles : %i ", meshFiles.size());

	// initialize zObjMesh
	operateMeshObjs.assign(meshFiles.size(), zObjMesh());

	// initialize & assign fnMesh
	fnOperateMeshes.assign(meshFiles.size(), zFnMesh());
	for (int i = 0; i < meshFiles.size(); i++)
		fnOperateMeshes[i] = zFnMesh(operateMeshObjs[i]);

	// initialize and create mySpectral 
	mySpectrals.assign(meshFiles.size(), zTsSpectral());
	for (int i = 0; i < meshFiles.size(); i++)
	{
		mySpectrals[i] = zTsSpectral(operateMeshObjs[i]);
		mySpectrals[i].createMeshfromFile(meshFiles[i], zJSON);
	}

	
	for (int i = 0; i < meshFiles.size(); i++)
	{
		//change color domain
		zDomainColor colDomain(zColor(1, 0, 0, 1), zColor(0, 0, 1, 1));
		mySpectrals[i].setColorDomain(colDomain);
		// add mesh to model for display
		model.addObject(operateMeshObjs[i]);
		operateMeshObjs[i].appendToBuffer();
	}

	
	// compute once in setup to get the laplacian values
	for (int i = 0; i < meshFiles.size(); i++)
		mySpectrals[i].computeEigenFunction(eigenFrequency);


	////// --- GUI  ----------------------------------------------------

	B = *new ButtonGroup(vec(50, 450, 0));
	B.addButton(&computeEigen, "compute Eigenfunc");
}

void update(int value)
{
	if (computeEigen)
	{
		for (int i = 0; i < meshFiles.size(); i++)
		{
			mySpectrals[i].computeEigenFunction(eigenFrequency);
			zColor* colors = fnOperateMeshes[i].getRawVertexColors();
			int id = fnOperateMeshes[i].getVBOVertexColorIndex();
			model.displayUtils.bufferObj.updateVertexColors(colors, fnOperateMeshes[i].numVertices(), id);
		}
	}

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	//drawGrid(20.0);

	B.draw();

	model.draw();

	setup2d();
	drawString("Press 'q' to increase the eigenFrequency", vec(50, 250, 0));
	drawString("Press 'a' to decrease the eigenFrequency", vec(50, 275, 0));
	drawString(("eigenFrequency: " + to_string((int)eigenFrequency)), vec(50, 300, 0));
	restore3d();
}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'q') eigenFrequency++;
	if (k == 'a')
	{
		eigenFrequency--;
		if (eigenFrequency < 0)	eigenFrequency = 0;
	}
}

void mousePress(int b, int state, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif _MAIN_