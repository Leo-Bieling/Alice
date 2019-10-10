#include "main.h"
#include "ALICE_ROBOT_DLL.h"
using namespace ROBOTICS;
#include <array>
#include <memory>
#include<time.h>
#include<experimental/generator> 

using namespace std;
using namespace std::experimental;

class particle
{

public:
	vec P; //object properties
	vec V; //object properties

	void action_setup() // initial condition
	{

		P = vec(ofRandom(10, -10), ofRandom(10, -10), ofRandom(10, -10)); //particle
		V = vec(ofRandom(10, -10), ofRandom(10, -10), ofRandom(10, -10)); // direction of travel
	}

	void action_move() //update rule
	{
		vec V_unit = V / V.mag(); //make vector move in direction of V


								  // condition to have point 'react' when it intersects the circle
		if (P.mag() < 20) P = P + V_unit * 0.1; // 2 = radius of circle (>2 outside of circle

		else //condition for vector intersetion on or inside circle
		{
			vec unit_normal = P / P.mag(); // normal is needed to find X in conjunction with Vt
			vec unit_normal_negative = unit_normal * -1; //switch direction of unit normal to add to tangent line to circle and get X
			vec Vn, Vt; // Vn = vector of normal Vt= tangent vector to circle
			float lenghtOfVn = V * unit_normal_negative; // Projection of -V in the opposite direction of the normal 
			Vn = unit_normal_negative * lenghtOfVn; // negative projection of Vn by multiplyig its length by the negative direction of normal
			Vt = V - Vn; // Vt = tanget vector to circle
			vec X = Vt + (Vn * -1); // vec X = direction of 'bounce off'use only Vt to make vector slide off of circle
			V = X; //update vector V to new direction
			P = unit_normal * 2.1; // take intersecting point and pushing out of the circle to make sure if condition continues

		}

	}

	void action_draw()
	{
		glPointSize(4);
		drawPoint(P); 
	}
};


vector<particle>collectionOfParticles; //creates a collection of particles



void setup()
{
	//create a particle and pushback

	for (int i = 0; i < 1000; i++) //define number of particles
	{
		particle a; //create a dummy particle
		collectionOfParticles.push_back(a);

	}

	for (auto &particle : collectionOfParticles)
	{
		particle.action_setup(); //execute setup action code in the class
	}
}


void update(int value)
{
	for (auto &particle : collectionOfParticles)
	{

		particle.action_move(); //execute move action coded in the class
	}
	
}



void draw()
{
	backGround(.75);
	//draw collection of particles

	for (auto &particle : collectionOfParticles)
	{
		
		particle.action_draw();
	}

	wireFrameOn();
	glutSolidSphere(20.0, 32, 32);
	wireFrameOff();

	

	
	
}
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'r') setup(); //restart code

}



void mousePress(int b, int state, int x, int y)
{
	
}

void mouseMotion(int x, int y)
{

}




