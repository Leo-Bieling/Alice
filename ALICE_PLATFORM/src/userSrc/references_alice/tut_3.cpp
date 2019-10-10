#include "main.h"


#define MaxTrail 10000 
#define MaxParticles 100 

vec pos[MaxParticles];

vec target;

vec trail[MaxTrail];
int trailCounter = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
	backGround(0.75, 0.75, 0.75, 0);
	
	// initialise points/particles
	for (int i = 0; i < MaxParticles; i++)
	{
		pos[i] = vec(ofRandom(0, 100), ofRandom(0, 100), 0);
	}
	
	//intialise target
	target = vec(ofRandom(0, 100), ofRandom(0, 100), 0);

	


}

void update(int value)
{
	// calculate new position 
	for (int i = 0; i < MaxParticles; i++)
	{
		//if (trailCounter < MaxTrail)
		//{

		
		// check for trail counter
		trail[trailCounter % MaxTrail] = pos[i];
		trailCounter = trailCounter + 1;
		//}
		
		// calculate disatance to target
		float distance = pos[i].distanceTo(target);

		//calculate direction to target
		vec dir = target - pos[i];

		//normailise direction
		dir.normalise();

		//add a small step in the direction to target to my current position
		pos[i] = pos[i] + dir * 0.1;

		pos[i].y = sin(10);

		
	}

	//printf(" trailCounter : %i ", trailCounter);
}


void draw()
{
	

	for (int i = 0; i < MaxParticles; i++)
	{
		glPointSize(5);
		drawPoint(pos[i]);
	}

	glColor3f(1, 0, 0);
	drawPoint(target);

	// check for trail counter
	int end = trailCounter;
	if (MaxTrail < trailCounter) end = MaxTrail;

	for (int i = 0; i < end; i++)
	{
		glColor3f(1, 1, 0); // open gl command for color
		glPointSize(0.1); // open gl command for pointsize
		drawPoint(trail[i]);
	}
}



void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 't')
	{
		//vel += gravity;
		//pos += vel;

		target = vec(ofRandom(0, 100), ofRandom(0, 100), 0);
	}


	if (k == 'l')
	{
	}
}

void mousePress(int b, int s, int x, int y)
{
	
}

void mouseMotion(int x, int y)
{
	target.x = x;
	target.y = y;
}




