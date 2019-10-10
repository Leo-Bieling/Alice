//#define _MAIN_

#ifdef _MAIN_

#include "main.h"
using namespace std;


void setup()
{
	for (int rows = 0; rows <= 5; rows++)
	{
		for (int hashes = 0; hashes < 6 - rows; hashes++)
		{
			cout << "#";
		}
		cout << endl;
	}
	
}

void update(int value)
{
}

void draw()
{
	backGround(0.75);
	drawGrid(20.0);

}

void keyPress(unsigned char k, int xm, int ym)
{
}

void mousePress(int b, int s, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif // _MAIN_