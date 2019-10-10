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
		
		setup2d();
		char jts[400];
	
		drawString(jts, winW * 0.5, winH - 100);

		int hts = 50;
		int wid = winW * 0.75;
		drawString(" n : path.goToNextPoint();", wid, hts); hts += 25;
		drawString(" b : path.goToPrev();", wid, hts); hts += 25;
		hts += 25;
		drawString("SPC :GS.smoothCurrentGraph()", wid, hts); hts += 25;
		
		restore3d();
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