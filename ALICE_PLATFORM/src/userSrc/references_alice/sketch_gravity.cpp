#define _MAIN_

#ifdef _MAIN_
#include "main.h"
#include "ALICE_ROBOT_DLL.h"
using namespace ROBOTICS;
#include <array>
#include <memory>
#include<time.h>
#include<experimental/generator> 

//using namespace std;
//using namespace std::experimental;

///////////////////////////////////////
//float minValue, maxValue;


///////////////////////////////////////


class particle
{
public:

	vec P; //object property
	vec V; //object porperty

	void action_setup()
	{
		P = vec(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(20, 40));
		V = vec(ofRandom(-1, 1), ofRandom(-1, 1), -1);
	}

	void action_move() // code to encode the action
	{
		vec gravity = vec(0, 0, -1);



		vec V_unit = V / V.mag();
		if (P.z > 0)
		{
			P = P + V_unit * 0.1;
			V += gravity * 0.1;
		}
		else
		{
			V.z = V.z * -1;
			P.z = P.z * -10;
		}
	}

	void action_draw()
	{
		glPointSize(3);
		glColor3f(1, 0, 0);
		drawPoint(P);
	}
};

//particle myparticles[1000];

vector<particle> collectionOfParticles;

void setup()
{
	for (int i = 0; i < 1000; i++)  // quantity of points
	{
		particle a;
		collectionOfParticles.push_back(a);
	}

	for (auto &particle : collectionOfParticles)
		particle.action_setup();

	/////////////////////////////////////////////

	/*for (int i = 0; i < 1000; i++)
	{
		vals[i] = ofRandom(0, 20);
	}*/

	/////////////////////////////////////////////

}

void update(int value)
{
	for (auto &particle : collectionOfParticles)
		particle.action_move();


}

void draw()
{
	backGround(0.75);
	
	//////////////////////////////////////////////////////////////


	for (int i = 0; i < collectionOfParticles.size(); i++)
	{
		vec4 clr = getColour(collectionOfParticles[i].P.z, 0, 64); // convert a single float number to a color value between the min and max values.

		glColor3f(clr.r, clr.g, clr.b); // use that color, to set the color the next point that will be drawn

		int sz = ofMap(collectionOfParticles[i].P.z, 0, 1, 1, 5); // remap the data range from minVlaue-maxValue range to 1-64 range.

		glPointSize(sz); // use the remapped value to set point size

		drawPoint(collectionOfParticles[i].P); // draw a point and at specific vector position ;

		
		
	}

	//cout << collectionOfParticles.size() << endl;

	glPopAttrib();


	//////////////////////////////////////////////////////////////
	
	for (auto &particle : collectionOfParticles)
		particle.action_draw();

	

}

void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'r') setup();
}

void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}
#endif
