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


///////// ----------------------------------------- model - view - controller (MVC) paradigm / pattern / template  ----------------- ////////////////////////////// 
/////////////////////////// model  ///////////////////////////////////////////
void setup()
{

}

void update(int value)
{

}

/////////////////////////// view  ///////////////////////////////////////////
void draw()
{

	backGround(0.75);
	drawGrid(20);

	/// default frame variables////
	vec TCP_x(1, 0, 0);
	vec TCP_y(0, 1, 0);
	vec TCP_z(0, 0, -1);
	vec target(10, 0, 0);

	///normalise the orientation vectors if they are not /////
	TCP_x.normalise();
	TCP_y.normalise();
	TCP_z.normalise();

	//// contruction of the frame ////
	Matrix4 TOOL;
	TOOL.setColumn(0, TCP_x);
	TOOL.setColumn(1, TCP_y);
	TOOL.setColumn(2, TCP_z);
	TOOL.setColumn(3, target);

	/// method of the class Nachi to calculate the invers kinemtics to reach the frame ////
	Nachi.inverseKinematics_analytical(TOOL, false);
}

/////////////////////////// control  ///////////////////////////////////////////
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