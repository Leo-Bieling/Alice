#include "main.h"


#define MaxTrail 1000 
#define MaxParticles 100



vec gravity(0, 0, -1);

vec pos[MaxParticles];
vec vel[MaxParticles];
vec forces[MaxParticles];

vec target;

vec trail[MaxParticles][MaxTrail];
int trailCounter = 0;

int frm = 0;




///// --------- SLIDERS & BUTTONS
SliderGroup S;
ButtonGroup B;

// slider variables
// note : to attach to slider the variable needs to be of type double
double targetStrength = 0.8;
double timeStep = 0.1; 
double gravityStrength = 0.0;
double spinStrength = 0.1;
double dragStrength = 0.01;
double collisonStrength = 0.1;
double collisonRadius = 5;

// button variables
// note : to attach to button the variable needs to be of type bool
bool run = false;

///// --------- METHODS

// PARTICLE to PARTICLE forces
//Collision
vec collision(int particleId1, int particleId2)
{
	float distance = pos[particleId1].distanceTo(pos[particleId2]);
	vec dir = pos[particleId2] - pos[particleId1];

	vec collisionForce(0, 0, 0);
	if (distance < collisonRadius)
	{
		collisionForce = dir * -1;
		collisionForce.normalise();
	}

	return collisionForce;

}

//EXTERNAL FORCES

//gravity
void applyGravity()
{
	for (int i = 0; i < MaxParticles; i++)
	{
		forces[i] += gravity * gravityStrength;
	}

}

//target force
void applyTargetForce()
{
	for (int i = 0; i < MaxParticles; i++)
	{
		float distance = pos[i].distanceTo(target);

		vec dir = target - pos[i];
		dir.normalise();
		//dir = dir * 0.1;

		forces[i] += dir * targetStrength; // forces[i] = forces[i] + dir
	}

}

// spin force
void applySpinForce()
{
	for (int i = 0; i < MaxParticles; i++)
	{
		float distance = pos[i].distanceTo(target);

		vec dir = target - pos[i];
		dir.normalise();
		//dir = dir * 0.1;

		//vec spinDir = vec(-dir.y, dir.x, 0); // only in 2d

		vec UpVec(0, 0, 1);

		vec spinDir = dir.cross(UpVec);



		forces[i] += spinDir * spinStrength; // forces[i] = forces[i] + dir
	}

}


// drag force
void applyDragForce()
{
	for (int i = 0; i < MaxParticles; i++)
	{
		vec dragDir = vel[i] * -1;

		forces[i] += dragDir * dragStrength; // forces[i] = forces[i] + dir
	}

}


//INTEGRATE
void integrateForces()
{
	for (int i = 0; i < MaxParticles; i++)
	{
		vec a = forces[i] / 1; // acceleration = force /mass
		vec newVel = vel[i] + a *timeStep; // new vel = old vel + acc * timestep

		vec newPos = pos[i] + newVel * timeStep; // new pos =  old position + newVelocity*timestep

		// update values
		pos[i] = newPos;
		vel[i] = newVel;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup()
{
	backGround(0.75, 0.75, 0.75, 0);

	// initialise points/particles & velocity
	for (int i = 0; i < MaxParticles; i++)
	{
		vel[i] = vec(0, 0, 0);
		pos[i] = vec(ofRandom(0, 100), ofRandom(0, 100), 0);
	}

	// initialise target
	target = vec(ofRandom(0, 100), ofRandom(0, 100), 0);


	// Sliders & Buttons
	S = *new SliderGroup();
	
	S.addSlider(&timeStep, "timeStep"); 
	S.sliders[0].minVal = 0.01;
	S.sliders[0].maxVal = 1.00;

	S.addSlider(&gravityStrength, "gravityStrength");
	S.sliders[1].minVal = 0.00;
	S.sliders[1].maxVal = 10.00;

	S.addSlider(&targetStrength, "targetStrength");
	S.sliders[2].minVal = 0.00;
	S.sliders[2].maxVal = 10.00;

	S.addSlider(&spinStrength, "spinStrength");
	S.sliders[3].minVal = 0.00;
	S.sliders[3].maxVal = 10.00;

	S.addSlider(&dragStrength, "dragStrength");
	S.sliders[4].minVal = 0.00;
	S.sliders[4].maxVal = 10.00;

	S.addSlider(&collisonStrength, "collisonStrength");
	S.sliders[5].minVal = 0.00;
	S.sliders[5].maxVal = 10.00;

	S.addSlider(&collisonRadius, "collisonRadius");
	S.sliders[6].minVal = 0.01;
	S.sliders[6].maxVal = 10.00;


	B = *new ButtonGroup(vec(50, 300, 0));
	
	B.addButton(&run, "run");
}

void update(int value)
{
	if (run)
	{
		//resetforces
		for (int i = 0; i < MaxParticles; i++)
		{
			forces[i] = vec(0, 0, 0);
		}

		// calculate particle forces
		for (int i = 0; i < MaxParticles; i++)
		{
			for (int j = i + 1; j < MaxParticles; j++)
			{
				if (i == j) continue;

				vec collisionForce = collision(i, j);
				//collisionForce.normalise();

				forces[i] += collisionForce * collisonStrength;

				forces[j] += collisionForce * collisonStrength * -1;


			}
		}

		// calculate forces
		applyGravity();
		applyTargetForce();
		applyDragForce();
		applySpinForce();



		// Update position or integration of forces
		integrateForces();


		//update trail
		for (int i = 0; i < MaxParticles; i++)
		{
			trail[i][frm%MaxTrail] = pos[i];
			
		}

		frm++;

	}

	

}


void draw()
{
	
	// set camera to Top View comment the line if you want 3d view
	topCamera();

	// SLiders & Buttons

	S.draw();
	B.draw();
	
	
	// particle Display
	for (int i = 0; i < MaxParticles; i++)
	{
		glColor3f(1, 1, 1);
		glPointSize(10);
		drawPoint(pos[i]);
	}

	// target Display
	glColor3f(1, 0, 0);
	drawPoint(target);

	// trail Display
	for (int i = 0; i < MaxParticles; i++)
	{
		int end = frm % MaxTrail;
		if (MaxTrail < frm ) end = MaxTrail;

		for (int j = 0; j < end; j++)
		{

			float val = ofMap(i, 0, MaxParticles, 0, 1);
			float valTrail = ofMap(j, 0, end, 0, 1);

			glColor4f(0, val, valTrail, valTrail);
			glPointSize(0.3);
			drawPoint(trail[i][j]);
		}
	}


	

}



void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 't')
	{
		target = vec(ofRandom(0, 10), ofRandom(0, 10), 0);
	}


	if (k == 'l')
	{
	}
}

void mousePress(int b, int s, int x, int y)
{
	if (GLUT_LEFT_BUTTON == b && GLUT_DOWN == s)
	{
		S.performSelection(x, y, HUDSelectOn);
		B.performSelection(x, y);
	}

}

void mouseMotion(int x, int y)
{

	S.performSelection(x, y, HUDSelectOn);
}