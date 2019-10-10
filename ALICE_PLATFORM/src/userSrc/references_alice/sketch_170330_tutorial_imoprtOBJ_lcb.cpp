#define _MAIN_

#ifdef _MAIN_

#include "main.h"
#include "MODEL.h"

#include <array>
#include <memory>
#include <time.h>
#include <experimental/generator> 
using namespace std;
using namespace std::experimental;

Mesh M;
Matrix3 rot;

///////// ----------------------------------------- model - view - controller (MVC) paradigm / pattern / template  ----------------- ////////////////////////////// 
/////////////////////////// model  ///////////////////////////////////////////
void setup()
{
	MeshFactory fac;
	M = fac.createFromOBJ("data/Site 01_Proposal Stage 03.obj", 1.0, false); //import obj

	rot.identity();
}

void update(int value)
{

}

/////////////////////////// view  ///////////////////////////////////////////
void draw()
{
		backGround(0.75);
		drawGrid(20);
		
		M.draw();
}

/////////////////////////// control  ///////////////////////////////////////////
void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

void keyPress(unsigned char k, int xm, int ym)
{
	vec u(1, 1, 0);
	vec v(-1, 1, 0);
	vec n = u.cross(v);

	u.normalise();
	v.normalise();
	n.normalise();

	rot.setColumn(0, u);
	rot.setColumn(1, v);
	rot.setColumn(2, n);

	for (int i = 0; i < M.n_v; i++) M.positions[i] = rot * M.positions[i];
}
#endif // _MAIN_