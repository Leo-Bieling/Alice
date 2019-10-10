#define _MAIN_

#ifdef _MAIN_
#include "main.h"
#include "ALICE_ROBOT_DLL.h"
using namespace ROBOTICS;
#include <array>
#include <memory>
#include<time.h>
#include<experimental/generator> 
using namespace std;
using namespace std::experimental;

#include "newPhysics.h"
#include "graph.h"
#include "gl2ps.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

importer imp;
Graph G;
Graph F;
newPhysics phy; // declare empty
int counter = 0;
//////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
	
	phy = *new newPhysics(); // calling a constructor with / without arguments ;

	imp = *new importer("data/pts.txt", 10000, 1.0);
	imp.readPts_p5();

	G = *new Graph();
	F = *new Graph();

	for (int i = 0; i < imp.nCnt; i++)
	{
		G.createVertex(imp.nodes[i].pos);
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j< 5; j++)
		{
			if (i == j)continue;

			int n1 = i;// ofRandom(0, G.n_v);
			int n2 = j;// ofRandom(0, G.n_v);

			G.createEdge( G.vertices[n1], G.vertices[n2] );
		}
	}

	cout << G.n_e << endl;
}



void update(int value)
{

	
}

void draw()
{

	backGround(0.75);

	
	G.draw();

}
void keyPress(unsigned char k, int xm, int ym)
{

	if (k == 'w')G.writeGraph(1.0,"data/nyGraph.txt");

	if (k == 'W')
	{

		string file = "data/";
		file += "out";
		file += "_";
		char s[20];
		itoa(counter, s, 10);
		file += s;
		file += ".txt";

		G.writeGraph(1.0, file );
		counter++;
	}

}

void mousePress(int b, int state, int x, int y)
{
	if (GLUT_LEFT_BUTTON == b && GLUT_DOWN == state)
	{
	

	}
}

void mouseMotion(int x, int y)
{
	//if (GLUT_LEFT_BUTTON == b && GLUT_DOWN == state)
	{
		//S.performSelection(x, y, HUDSelectOn);
	}
}
#endif
