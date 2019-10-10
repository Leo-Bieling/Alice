#define _MAIN_

#ifdef _MAIN_
#include "main.h"
#include "ALICE_ROBOT_DLL.h"
using namespace ROBOTICS;

Robot_Symmetric Nachi;

/////ORIENTATION/////

vec TCP_x(1, 0, 0);
vec TCP_y(0, 1, 0);
vec TCP_z(0, 0, -1);


/////LOCATION/////
vec target(40, 0, 0);


void setup()
{
	Nachi.addMeshes();

	Nachi.rot[3] = 120;
}

void update(int value)
{
	Nachi.ForwardKineMatics(Nachi.rot);
}

void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	glPointSize(5);
	drawPoint(target);

	Nachi.draw(true);
	
}

void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'n')
	{
		for (int i = 0; i < DOF; i++) Nachi.rot[i] = 0;

		TCP_x.normalise();
		TCP_y.normalise();
		TCP_z.normalise();

		Matrix4 frame;
		frame.setColumn(0, TCP_x);
		frame.setColumn(1, TCP_y);
		frame.setColumn(2, TCP_z);
		frame.setColumn(3, target + vec(0,0,26));

		Nachi.inverseKinematics_analytical(frame, true);
	}



}

void mousePress(int b, int state, int x, int y)
{

}
void mouseMotion(int x, int y)
{

}


#endif // DEBUG
