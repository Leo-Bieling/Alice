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

vec P, V, S;

class particle
{
public:

	vec P, V, S; //vectors and spring force

	void setup()
	{
		P = vec(ofRandom(-20, 20), ofRandom(-20, 20), 0); //initial position of particles
		V = vec(0, 0, 0); //P / P.mag(); //direction of motion
		S = vec(0, 0, 0); //location of spring force
	}

	void update(vector<particle>COP)
	{
		// set the force of the spring from one point to each particle location
		// update P, V
		addSpring(COP);
		updatePositionAndVelocity();
	}

	void addSpring(vector<particle>COP)
		{
		for (auto &particle : COP)
		{
				vec Ps = particle.P; //particle in the collection
				vec D = (Ps - S);
				float L = (Ps - S).mag();
				vec D_unit = D / D.mag();
				float original = 1;
				float K = .1;
				float spring_force = (original - L) * K;
				vec Spring = D_unit * spring_force;
				V = V + D_unit + Spring;
				
				for (auto &particle : COP)
				{
					//Ps = Ps + V;
					Ps = P + V;
				}
				
				//float D = (Ps - S).mag(); //lenght between spring and particle
				//float original = 1; //original spring lenght
				//vec E = Ps - S / (Ps - S).mag(); //direction of motion
				//float K = .001; //spring constant
				//float S_force = (original - D)* K; //force of spring 
				//vec Spring = E * S_force; // vector of spring
			
				
			//V =  D_unit + Spring; // && D < 11 if (D > 1 && D < 2)
			
			}
		}

		void updatePositionAndVelocity() //property of the particle within the class. 
		{
			
			//float deltaT = 0.01; //moment in time of vector motion
			//vec deltaP = V * deltaT; // update of P position in relation to its velocity at a certain moment in time
			//P = P + deltaP; //update rule for P

			vec V_unit = V / V.mag();
			P = P + V_unit * 0.1;
		}

		void draw()
		{
			drawPoint(P);
			//drawLine(P, P + V);
			drawLine(P, S);
		}	
};

vector<particle> COP;
bool run = false;

void setup()
{
	for (int i = 0; i < 20; i++) // particles to push back to collection
	{
		particle a;
		COP.push_back(a);
	}

	for (auto &particle : COP)particle.setup(); //setup of class	
}


void update(int value)
{
	if (run == true) //if animation is playing then run update (include arguments in orginal 'update' function)
		for (auto &p : COP)p.update( COP);//pass by copy	
}

void draw()
{
	backGround(0.75);
	glPointSize(5);

	for (auto &prt : COP)prt.draw();

	//glColor3f(1, 0, 0);
	//drawPoint(vec (1,1,0));
}
	
void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'r')setup();
	if (k == 'p')run = !run;
}

void mousePress(int b, int state, int x, int y)
{
	
}

void mouseMotion(int x, int y)
{

}
#endif
