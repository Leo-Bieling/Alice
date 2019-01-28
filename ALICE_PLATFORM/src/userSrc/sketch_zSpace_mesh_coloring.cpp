//#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zData.h>
#include <headers/include/zIO.h>

using namespace zSpace;
using namespace std;

// function to remap doubles
double remap_value(double value, double max_from, double min_from, double max_to, double min_to)
{
	return min_to + (value - min_from) * (max_to - min_to) / (max_from - min_from);
}

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------
////// --- MODEL OBJECTS ----------------------------------------------------
zMesh myMesh;
vector<zVector> face_center;
vector<double> center_z_values;
vector<zColor> remapped_colors;
bool color_z_value = false;
bool faceID = false;

////// --- GUI OBJECTS ----------------------------------------------------

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	// import mesh from obj
	fromOBJ(myMesh, "data/zSpace_mesh_coloring_test.obj");
	
	// setup button group
	B = *new ButtonGroup;
	B.addButton(&color_z_value, "show mapped z values");
	B.addButton(&faceID, "show faceIDs");

	// get center points of each mesh face
	getCenters(myMesh, zFaceData, face_center);

	// get z value of each center point
	for (int i = 0; i < face_center.size(); i++) center_z_values.push_back(face_center[i].z);

	// get the max and min value in the container
	double max = *max_element(center_z_values.begin(), center_z_values.end());
	double min = *min_element(center_z_values.begin(), center_z_values.end());

	// create container with remapped zColor values
	for (int i = 0; i < center_z_values.size(); i++) remapped_colors.push_back(zColor(remap_value(center_z_values[i], max, min, 1, 0), remap_value(center_z_values[i], max, min, 0, 1), 0, 1));
}

void update(int value)
{
	//if (color_z_value)
	//{
	//	// apply the remapped color container to the mesh
	//	setFaceColors(myMesh, remapped_colors, false);
	//}
	//else
	//{
	//	// set all face colors to grey
	//	setFaceColor(myMesh, zColor(0.4, 0.4, 0.4, 1));
	//}

	// shorthand version of the if - else statement above
	(color_z_value) ? setFaceColors(myMesh, remapped_colors, false) : setFaceColor(myMesh, zColor(0.4, 0.4, 0.4, 1));
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	// show faceID
	if (faceID)
	{
		for (int i = 0; i < face_center.size(); i++)
		{
			string s = to_string(i);
			drawString(s, vec(face_center[i].x, face_center[i].y, face_center[i].z));
		}

		for (int i = 0; i < face_center.size(); i++) drawPoint(face_center[i]);
	}


	// draw buttons
	B.draw();

	// draw mesh
	drawMesh(myMesh, false, true, true);
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