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

////////////////////////////////////////////////////////////////////////// GLOBAL VARIABLES ----------------------------------------------------


////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------


////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{


}

void update(int value)
{

}


////// ---------------------------------------------------- VIEW  ----------------------------------------------------

void draw()
{
	backGround(0.75);
	drawGrid(20.0);

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