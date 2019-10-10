#include "main.h"
#include "ALICE_ROBOT_DLL.h"
using namespace ROBOTICS;
#include <array>
#include <memory>
#include<time.h>
#include<experimental/generator> 

using namespace std;
using namespace std::experimental;

vec A = vec(10, 10, 10);
vec B = vec(5, 5, 5);

void setup()
{
	
	

}

void update(int value)
{
	
	A.x = A.x * -1;

A.y = A.y * -1;
	

A.z = A.z * -1;
	


		B.x += B.y * -.1;
		B.y += B.x * .1;


}



void draw()
{

	backGround(0.75);

	glPointSize(10);
	drawPoint(A);
	glPointSize(20);
	drawPoint(B);
	drawCircle(B, 4, 20);
	drawLine(A,B);
		


}
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'm'){
		A.x = A.x * -1; 
	}

	if (k == 'n') {
		A.y = A.y * -1;
	}

	if (k == 'b') {
		A.z = A.z * -1;
	}

	if (k == 'q') {
		B.x += B.y * -.1;
		B.y += B.x * .1;
	}
}



void mousePress(int b, int state, int x, int y)
{
	
}

void mouseMotion(int x, int y)
{

}




