#define _MAIN_

#ifdef _MAIN_

#include "main.h"
#include "MODEL.h"
#include <array>
#include <memory>
#include "time.h"
#include <experimental/generator>
#include "dwellt_unit.h""
using namespace std;
using namespace std::experimental;

// Import
Mesh buildings;
Mesh plot;
Mesh proposalStage1;
Mesh proposalStage2;
Mesh proposalStage3;
Mesh sporadicUser;
Mesh recurringUser;
Mesh constantUser;
bool buildingsOnOff = true;
bool plotOnOff = true;
bool proposalStage1OnOff;
bool proposalStage2OnOff;
bool proposalStage3OnOff;
bool sporadicUserOnOff;
bool recurringUserOnOff;
bool constantUserOnOff;

// Plot
vec plotA;
vec plotB;
vec plotC;
vec plotD;

// Core
bool coreOnOff;
float coreW = 2;
float coreD = 2;
float maxHeight; // defined by surrounding buildings
float maxHeightScale = 1.5;


// Unit 1 TEST
int unit1posX;
int unit1posY;
int unit1posZ;
int unit1dimX = 4;
int unit1dimY = 2;
int unit1dimZ = 2.6;
// List of Units 1 TEST
vector<vec> collectionOfUnits;


// Final proposal with units
bool socialOnOff;
bool proposalOnOff;
vector<units> collectionsOfUnits2;
units unit0;
units unit1;
units unit2;

// Plot
int maxLines = 1000;
vector<vec> pts;


//----------------------    unit1 function TEST    -----------------------------------------------------------------------------------
void drawU1(int _unit1posX, int _unit1posY, int _unit1posZ)
{
	glPushMatrix();
	glTranslatef(_unit1posX, _unit1posY, _unit1posZ);
	glLineWidth(6);
	wireFrameOn();
	drawCube(vec(0, 0, 0), vec(unit1dimX, unit1dimY, unit1dimZ));
	wireFrameOff();
	glLineWidth(1);
	glPopMatrix();
}
//-----------------------------------------------------------------------------------------------------------------------------------



///////// ----------------------------------------- model - view - controller (MVC) paradigm / pattern / template  ----------------- ////////////////////////////// 
/////////////////////////// model  ///////////////////////////////////////////
void setup()
{
	MeshFactory fac;

	buildings = fac.createFromOBJ("data/Site 01_Buildings.obj", 1.0, false);
	plot = fac.createFromOBJ("data/Site 01_Plot.obj", 1.0, false);

	proposalStage1 = fac.createFromOBJ("data/Site 01_Proposal Stage 01.obj", 1.0, false);
	proposalStage2 = fac.createFromOBJ("data/Site 01_Proposal Stage 02.obj", 1.0, false);
	proposalStage3 = fac.createFromOBJ("data/Site 01_Proposal Stage 03.obj", 1.0, false);

	sporadicUser = fac.createFromOBJ("data/Site 01_Sporadic.obj", 1.0, false);
	recurringUser = fac.createFromOBJ("data/Site 01_Recurring.obj", 1.0, false);
	constantUser = fac.createFromOBJ("data/Site 01_Constant.obj", 1.0, false);


	unit0 = *new units(vec(0, -10, 0), 0);
	unit1 = *new units(vec(4, -10, 0), 1);
	unit2 = *new units(vec(9, -10, 0), 2);

	/////////////////////////////////////////////////////////////////////
	// open the appropriate file for reading
	std::fstream fs("data/pointsOfPlotOutline.txt", ios::in);

	// set initial number of lines to 0;
	int numLines = 0;

	// while the file has content left to be read (i.e. the end of file has NOT been reached ), repeatedly perform these actions
	/*
	1. get the current line from the open file
	2. convert contents of the line into coordinates and then to a point
	3. store point in pts container
	*/
	while (!fs.eof() && numLines < maxLines) // the second condition is a safety check, in case the file is corrupt
	{
		char str[2000]; // a storage container to store characters
		fs.getline(str, 2000); // get the current line from the open file, store contents of line in str (character array);
		vector<string> content = splitString(str, ","); // collect characters into strings, by looking for comma separators in the character array;

		if (content.size() != 3)continue; // if the number of strings is not equal 3, skip and go to next line

		float x = atof(content[0].c_str()); // convert the first string into characters (.c_str()), then convert character to float (atof);
		float y = atof(content[1].c_str());// convert the second string into characters (.c_str()), then convert character to float (atof);
		float z = atof(content[2].c_str());// convert the thrid string into characters (.c_str()), then convert character to float (atof);

		vec curPt = vec(x, y, z); // create a point(vector) from floats;
		pts.push_back(curPt); // push back the Point into the storage container (vector - pts)

		numLines++; // increment the counter keeping track of number of lines.
	}

	fs.close(); // close file;
}

void update(int value)
{
	
}

/////////////////////////// view  ///////////////////////////////////////////
void draw()
{
	backGround(0.9);
	drawGrid(20);

	
	//-------------- SITE & PLOT --------------//
	glPushMatrix();
	glTranslatef(-2.59113, -5.64213, 0);
	if (buildingsOnOff) buildings.draw(); //BUILDING
	vec BoundingBoxMax;
	vec BoundingBoxMin;
	buildings.boundingBox(BoundingBoxMin, BoundingBoxMax);
	maxHeight = BoundingBoxMax.z; //getting the maximum height of surrounding buildings
	glPopMatrix();

	if (plotOnOff)
	{
		///----- drawLines
		glLineWidth(3);
		glColor3f(0, 0, 0);
		lineStyle(3);
		int n = pts.size();
		for (int i = 0; i < n; i++)
		{
			int next = (i + 1) % n; // a formula to get the next number to the current in a cyclical way : i.e next of 1 = 2, next of last point = 0;
									// % is the modulo operation, and returns thee remainder when the numerator( i+1) is divided by n;
									// thus when i + 1 == n, the remainder is 0;
			drawLine(pts[i], pts[next]); // draw line between current point and next point;
		}
		glLineWidth(1);
		lineStyle(0);

		///----- drawPoints
		glPointSize(15);
		glColor3f(1, 0, 0);
		for (int i = 0; i < n; i++) drawPoint(pts[i]);
		glPointSize(1);
		glColor3f(1, 1, 1);
	}

	//--------- PROPOSAL REFERENCE--------//
	glPushMatrix();
	glTranslatef(2.30073, -0.106446, 0);
	if (proposalStage1OnOff) proposalStage1.draw(); //STAGE 1
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.80073, -2.10645, 0);
	if (proposalStage2OnOff) proposalStage2.draw(); //STAGE 2
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.30073, 1.39355, 0);
	if (proposalStage3OnOff) proposalStage3.draw(); //STAGE 3
	glPopMatrix();

	//-------------- USER --------------//
	glPushMatrix();
	glTranslatef(1, -5, 0);
	if (sporadicUserOnOff) sporadicUser.draw(); //SPORADIC
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, -5, 0);
	if (recurringUserOnOff) recurringUser.draw(); //RECURRING
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, -5, 0);
	if (constantUserOnOff) constantUser.draw(); //CONSTANT
	glPopMatrix();

	//---------------------- LEGEND ----------------------//
	setup2d();
	glColor3f(1, 0, 1);
	int hts = 50;
	int wid = winW * 0.75;
	drawString("q : Buildings;", wid, hts); hts += 25;
	drawString("w : Plot", wid, hts); hts += 50;
	drawString("r : Stage 1 Reference", wid, hts); hts += 25;
	drawString("t : Stage 2 Reference", wid, hts); hts += 25;
	drawString("y : Stage 3 Reference", wid, hts); hts += 50;
	drawString("a : Sporadic User", wid, hts); hts += 25;
	drawString("s : Recurring User", wid, hts); hts += 25;
	drawString("d : Constant User", wid, hts); hts += 50;
	drawString("p : Unit Proposal", wid, hts); hts += 25;
	drawString("[ : Core Proposal", wid, hts); hts += 25;
	drawString("] : Social Unit Proposal", wid, hts); hts += 25;


	glColor3f(0, 0, 0);
	restore3d();

	//----------------------------------------------------//

	// draw plot lables
	glColor3f(1, 0, 1);
	plotA = pts[3];
	plotB = pts[0];
	plotC = pts[1];
	plotD = pts[2];

	drawString("A", plotA);
	drawString("B", plotB);
	drawString("C", plotC);
	drawString("D", plotD);

	vec plotAD = (plotA + plotD) * 0.5;
	drawString("AD", plotAD);
	glColor3f(0, 0, 0);

	// draw core
	if (coreOnOff)
	{
		maxHeight *= maxHeightScale;
		wireFrameOn();
		drawCube(vec(plotAD.x - coreW*0.5, plotA.y, 0), vec(plotAD.x + coreW*0.5, plotA.y + coreD, maxHeight)); //CORE placed according to plot
		wireFrameOff();
	}

	// draw social units
	if (socialOnOff)
	{
		for (int i = 0; i < maxHeight / 2.6; i++)
		{
			float factor = 0.05;
			glColor3f(1, 0, 1);
			glLineWidth(4);
			drawLine(plotA*(0.5 + i * factor) + vec(0, 0, 2.6 * i), plotB*(0.5 + i * factor) + vec(0, 0, 2.6 * i));
			drawLine(plotB*(0.5 + i * factor) + vec(0, 0, 2.6 * i), plotC*(0.5 + i * factor) + vec(0, 0, 2.6 * i));
			drawLine(plotC*(0.5 + i * factor) + vec(0, 0, 2.6 * i), plotD*(0.5 + i * factor) + vec(0, 0, 2.6 * i));
			drawLine(plotD*(0.5 + i * factor) + vec(0, 0, 2.6 * i), plotA*(0.5 + i * factor) + vec(0, 0, 2.6 * i));
			glLineWidth(1);
			glColor3f(0, 0, 0);
		}
	}

	// draw Units TEST
	for (int i = 0; i < collectionOfUnits.size(); i++)
	{
		drawU1(collectionOfUnits[i].x, collectionOfUnits[i].y, collectionOfUnits[i].z);
	}

	if (proposalOnOff)
	{
		unit0.draw();
		unit1.draw();
		unit2.draw();
	}
}

/////////////////////////// control  ///////////////////////////////////////////
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'q')buildingsOnOff = !buildingsOnOff;
	if (k == 'w')plotOnOff = !plotOnOff;

	if (k == 'r')proposalStage1OnOff = !proposalStage1OnOff;
	if (k == 't')proposalStage2OnOff = !proposalStage2OnOff;
	if (k == 'y')proposalStage3OnOff = !proposalStage3OnOff;

	if (k == 'a')sporadicUserOnOff = !sporadicUserOnOff;
	if (k == 's')recurringUserOnOff = !recurringUserOnOff;
	if (k == 'd')constantUserOnOff = !constantUserOnOff;

	if (k == 'p')proposalOnOff = !proposalOnOff;
	if (k == '[')coreOnOff = !coreOnOff;
	if (k == ']')socialOnOff = !socialOnOff;


	if (k == '6')
	{
		unit1posX += unit1dimX;
		collectionOfUnits.push_back(vec(unit1posX, unit1posY, unit1posZ));
	}
	if (k == '4')
	{
		unit1posX -= unit1dimX;
		collectionOfUnits.push_back(vec(unit1posX, unit1posY, unit1posZ));
	}
	if (k == '8')
	{
		unit1posY += unit1dimY;
		collectionOfUnits.push_back(vec(unit1posX, unit1posY, unit1posZ));
	}
	if (k == '5')
	{
		unit1posY -= unit1dimY;
		collectionOfUnits.push_back(vec(unit1posX, unit1posY, unit1posZ));
	}
	if (k == '9')
	{
		unit1posZ += unit1dimY;
		collectionOfUnits.push_back(vec(unit1posX, unit1posY, unit1posZ));
	}
	if (k == '3')
	{
		unit1posZ -= unit1dimY;
		collectionOfUnits.push_back(vec(unit1posX, unit1posY, unit1posZ));
	}
}

void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}
#endif // _MAIN_