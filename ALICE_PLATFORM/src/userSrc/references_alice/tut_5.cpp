#include "Particles.h"
#include "gl2ps.h"

#ifndef MAIN
#define MAIN
	#include "main.h"
#endif





#define MaxParticles 100
#define MaxTargets 10

////// VARIABLES
vec gravity(0, 0, -1);

myParticle particles[MaxParticles];

//vec target;
myTarget mytargets[MaxTargets];

//vec trail[MaxParticles][MaxTrail];
int trailCounter = 0;
int frm = 0;




///// --------- SLIDERS & BUTTONS
SliderGroup S;
ButtonGroup B;

// slider variables
// note : to attach to slider the variable needs to be of type double
double targetStrength = 0.8;
double magnetStrength = 2.0;
double timeStep = 0.1;
double gravityStrength = 0.01;
double spinStrength = 0.1;
double dragStrength = 0.01;
double collisonStrength = 0.1;
double collisonRadius = 5;

double ang = 0;

// button variables
// note : to attach to button the variable needs to be of type bool
bool run = false;

///// --------- METHODS





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup()
{
	backGround(0.75, 0.75, 0.75, 0);

	// initialise points/particles & velocity
	for (int i = 0; i < MaxParticles; i++)
	{
		particles[i].v = vec(0, 0, 0);
		particles[i].p = vec(ofRandom(0, 100), ofRandom(0, 100), 0);
	}

	// initialise target

	for (int i = 0; i < MaxTargets; i++)
	{
		mytargets[i].tarPos = vec(ofRandom(0, 100), ofRandom(0, 100), 0);
		
		//if (i % 2 == 0)mytargets[i].charge *= -1;

	}

	

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

	S.addSlider(&magnetStrength, "magnetStrength");
	S.sliders[7].minVal = 0.01;
	S.sliders[7].maxVal = 20.00;


	B = *new ButtonGroup(vec(50, 500, 0));

	B.addButton(&run, "run");
}

void update(int value)
{
	if (run)
	{
		// calculate  & integrate forces
		
		for (int i = 0; i < MaxParticles; i++)
		{

			// gravity
			//particles[i].applyGravity(gravity, gravityStrength);

			//drag
			particles[i].applyDragForce(dragStrength);

			//target related forces
			for (int j = 0; j < MaxTargets; j++)
			{
				//target forces
				mytargets[j].applyTargetForce(particles[i], targetStrength);

				//magnetic force
				//mytargets[j].applyMagneticForce(particles[i], magnetStrength);

				//spin force
				mytargets[j].applySpinForce(particles[i], spinStrength);
			}


			//Integrate forces
			particles[i].integrateForces(timeStep);

			//update trail
			particles[i].myTrail[frm % MaxTrail] = particles[i].p;


			
		}


		frm++;

	}



}


void draw()
{

	// set camera to Top View comment the line if you want 3d view
	//topCamera();

	// SLiders & Buttons

	S.draw();
	B.draw();


	// particle Display
	for (int i = 0; i < MaxParticles; i++)
	{
		glColor3f(1, 1, 1);
		glPointSize(10.0);
		drawPoint(particles[i].p);

		// trail Display
		int end = frm % MaxTrail;
		if (MaxTrail < frm) end = MaxTrail;

		for (int j = 0; j < end; j++)
		{

			float val = ofMap(i, 0, MaxParticles, 0, 1);
			float valTrail = ofMap(j, 0, end, 0, 1);

			glColor4f(0, val, valTrail, valTrail);
			glPointSize(0.3);
						
			drawPoint(particles[i].myTrail[j]);
		}

	}

	// target Display

	for (int i = 0; i < MaxTargets; i++)
	{
		glColor3f(1, 0, 0);
		drawPoint(mytargets[i].tarPos);
	}




}



void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 't')
	{
		mytargets[0].charge *= -1;
	}


	if (k == 'l')
	{
	}


	if (k == 'A') // to print high res EPS image
	{
		FILE *fp;
		int state = GL2PS_OVERFLOW, buffsize = 0;
		fp = fopen("out.eps", "w");
		printf("Writing 'out.eps'... ");

		while (state == GL2PS_OVERFLOW)
		{
			buffsize += winW * winH;
			gl2psBeginPage("test", "gl2psTestSimple", NULL, GL2PS_EPS, GL2PS_SIMPLE_SORT,
				GL2PS_USE_CURRENT_VIEWPORT,
				GL_RGBA, 0, NULL, 0, 0, 0, buffsize, fp, "out.eps");

			draw();

			state = gl2psEndPage();
		}




		fclose(fp);
		printf("Done!\n");

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