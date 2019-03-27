//#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zData.h>
#include <headers/include/zIO.h>

#include <userSrc/perlinNoise.h>

using namespace zSpace;
using namespace perlin;
using namespace std;




////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
double yOffset = 0;
double increments = 0.1;

double moveThrough = 0;

int width = 200;
int height = 200;
int scale = 5;
float cols, rows;
zField2D<double> field;
zMesh fieldMesh;
vector<zVector> fieldPostions;

vector<zColor> fColors;
vector<zVector> faceCenters;
vector<zMesh> test;

////// --- GUI OBJECTS ----------------------------------------------------


////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	////////////////////////////////////////////////////////////////////////// Enable smooth display

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);


	cols = floor(width / scale);
	rows = floor(height / scale);

	zVector minBB = zVector(-width / 2, -height / 2, 0);
	zVector maxBB = zVector(width / 2, height / 2, 0);
	field = zField2D<double>(minBB, maxBB, cols, rows);
	for (int i = 0; i < field.fieldValues.size(); i++)		
		fieldPostions.push_back(field.getPosition(i));
	
	
	from2DFIELD(fieldMesh, field);
	   
	getCenters(fieldMesh, zFaceData, faceCenters);

	S = *new SliderGroup();
	S.addSlider(&moveThrough, "y offset");
	S.sliders[0].attachToVariable(&moveThrough, 0, 100);
	S.addSlider(&increments, "increments");
	S.sliders[1].attachToVariable(&increments, 0.001, 1);

}

void update(int value)
{
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);

	yOffset = moveThrough;

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
	drawMesh(fieldMesh, false, true, true);
	fColors.clear();	
	
	for (int i = 0; i < field.fieldValues.size(); i++)
	{
		drawRectangle(zVector(fieldPostions[i].x + (scale/2), fieldPostions[i].y + (scale / 2), ofMap(fieldMesh.faceColors[i].r, 0, 1, -15, 75)), zVector(fieldPostions[i].x - (scale / 2), fieldPostions[i].y - (scale / 2), ofMap(fieldMesh.faceColors[i].r, 0, 1, -15, 75)));
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