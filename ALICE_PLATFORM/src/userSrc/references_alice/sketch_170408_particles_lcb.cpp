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

class particle
{
public:

	vec P;
	vec V;

	Mesh User;

	particle() {};
	particle(int x, int y, int z, float _speed)
	{
		P = vec(ofRandom(-x*0.5, x*0.5), ofRandom(-y*0.5, y*0.5), ofRandom(0, z));
		V = vec(ofRandom(-_speed, _speed), ofRandom(-_speed, _speed), ofRandom(-_speed, _speed));
		
		MeshFactory fac;
		User = fac.createFromOBJ("data/Site 01_Sporadic.obj", 1.0, false);
	}

	void draw()
	{
		glColor3f(1, 0, 1);
		glPointSize(4);
		drawPoint(P);
		glPointSize(1);
		glColor3f(0, 0, 0);

		glPushMatrix();
		glTranslatef(P.x, P.y, P.z);
		User.draw();
		glPopMatrix();
	}

	void addVelocity(int _frameWidth, int _frameDepth, int _frameHeight)
	{
		P += V;
		if (P.z <= 0) V.z *= -1;
		if (P.z >= _frameHeight) V.z *= -1;
		if (P.x <= _frameDepth * -0.5) V.x *= -1;
		if (P.x >= _frameDepth * 0.5) V.x *= -1;
		if (P.y <= _frameWidth * -0.5) V.y *= -1;
		if (P.y <= _frameWidth * 0.5) V.y *= -1;
	}
};

int frameWidth = 40;
int frameDepth = 40;
int frameHeight = 40;
float speed = 0.2;
int numberOfParticles = 100;
vector <particle> collectionOfParticles;
particle P;

///////// ----------------------------------------- model - view - controller (MVC) paradigm / pattern / template  ----------------- ////////////////////////////// 
/////////////////////////// model  ///////////////////////////////////////////
void setup()
{
	for (int i = 0; i < numberOfParticles; i++)
	{
		P = *new particle(frameWidth, frameDepth, frameHeight, speed);
		collectionOfParticles.push_back(P);
	}
}

void update(int value)
{
	for (int i = 0; i < collectionOfParticles.size(); i++)
	{
		collectionOfParticles[i].addVelocity(frameWidth, frameDepth, frameHeight);
	}
}

/////////////////////////// view  ///////////////////////////////////////////
void draw()
{

	backGround(0.75);
	drawGrid(20);

	wireFrameOn();
	drawCube(vec(frameWidth * -0.5, frameDepth * -0.5, 0), vec(frameWidth * 0.5, frameDepth * 0.5, frameHeight));
	wireFrameOff();

	for (int i = 0; i < collectionOfParticles.size(); i++)
	{
		collectionOfParticles[i].draw();
		drawLine(collectionOfParticles[i].P, collectionOfParticles[i - 1].P);
	}

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