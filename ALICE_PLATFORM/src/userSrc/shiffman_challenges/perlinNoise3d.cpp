//#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <userSrc/shiffman_challenges/perlinNoise.h>

#include <headers/include/zCore.h>
#include <headers/include/zData.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zIO.h>


using namespace zSpace;
using namespace perlin;
using namespace std;


double increments = 0.05;

int width = 100;
int height = 100;
int scale = 1;
float cols, rows;
zField2D<double> field;
zMesh fieldMesh;

vector<zColor> fColors;
vector<zVector> faceCenters;

////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{

	cols = floor(width / scale);
	rows = floor(height / scale);

	zVector minBB = zVector(-width/2, -height/2, 0);
	zVector maxBB = zVector(width/2, height/2, 0);
	field = zField2D<double>(minBB, maxBB, cols, rows);
	from2DFIELD(fieldMesh, field);

	getCenters(fieldMesh, zFaceData, faceCenters);


}

void update(int value)
{
	
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);

	double yOffset = 0;



	for (int i = 0; i < rows; i++)
	{
		double xOffset = 0;

		for (int j = 0; j < cols; j++)
		{
			field.setFieldValue(ofMap(noise(xOffset, yOffset), -1, 1, 0, 1), i);
			fColors.push_back(zColor(field.getFieldValue(i), field.getFieldValue(i), field.getFieldValue(i), 1));
			xOffset += increments;
		}
		yOffset += increments;
	}
	
	setFaceColors(fieldMesh, fColors);
	//drawMesh(fieldMesh, false, false, true);
	fColors.clear();

	for (int i = 0; i < faceCenters.size(); i++)
	{
		drawLine(faceCenters[i], faceCenters[i] + zVector(sin(fieldMesh.faceColors[i].r*scale), cos(fieldMesh.faceColors[i].r*scale), 0), fieldMesh.faceColors[i]);
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

#endif _MAIN_