//#define _MAIN_
/*
Try doing the rainflow in this way :
	-make a vector and colour the mesh based on the distance from the vector to each face center
	-start from a face center and descend the color gradient = check the lowest value in the connected faces, move there and make an edge
	- repeat
	- stop if the near values are higher then current value

for the gradient you can import an OBJ using this :
	string path = "data/InputMesh.obj";
importOBJ_HE(inputMesh, path);
*/

#ifdef _MAIN_


#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zData.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zIO.h>

using namespace zSpace;

zMesh inputMesh;
zBufferObject Buffer;

double power = 1.0;

bool compute = false;

vector<zVector> centers;

string s = "Press T: 2D Camera";
string s1 = "Press F: Screenshot";


void setup()
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);

	/// Initialize Buttons and Sliders
	S = *new SliderGroup(vec(50, 50, 0));
	B = *new ButtonGroup(vec(50, 500, 0));

	/// Create zMesh

	/// Vertices positions
	vector<zVector> positions;
	positions.push_back(zVector(0, 0, 0));
	positions.push_back(zVector(10, 0, 0));
	positions.push_back(zVector(10, 10, 0));
	positions.push_back(zVector(0, 10, 0));

	/// Number of vertices per polygon face
	vector<int> polyCounts;
	polyCounts.push_back(4);

	/// Vertices connectivity (sequence order fundamental! always CLOCKWISE)
	vector<int> polyConnects;
	polyConnects.push_back(0);
	polyConnects.push_back(1);
	polyConnects.push_back(2);
	polyConnects.push_back(3);

	/// Overload constructor
	inputMesh = zMesh(positions, polyCounts, polyConnects);

	inputMesh.faceColors[0] = zColor(1, 0, 0, 1);

	/// Create buffer
	Buffer = zBufferObject(100000);

	/// Get centers of zData structure
	getCenters(inputMesh, zFaceData, centers);

	/// Get Connected Components
	vector<int> ids;
	int faceId = inputMesh.faces[0].getFaceId();
	inputMesh.getConnectedFaces(faceId, zFaceData, ids);
	
	Buffer.appendMesh(inputMesh);
}


void update(int value)
{
	if (compute)
	{
		inputMesh.computeVertexColorfromFaceColor();
		inputMesh.computeEdgeColorfromVertexColor();

		Buffer.updateVertexColors(inputMesh.vertexColors, inputMesh.VBO_VertexId);
	}
}

void draw()
{

	backGround(0.50);
	drawGrid(50.0);

	S.draw();
	B.draw();

	drawLinesFromBuffer(Buffer, true);
	drawQuadsFromBuffer(Buffer, true);

	setup2d();

	glColor3f(0, 0, 0);

	drawString(s.c_str(),50, 800);
	drawString(s1.c_str(), 50, 850);

	restore3d();
}

void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'e')
	{
		string out = "data/Mesh.json";
		toJSON(inputMesh, out, true);
	}
}

void mousePress(int b, int s, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

#endif // _MAIN_
