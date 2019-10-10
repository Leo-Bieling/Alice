#ifndef MY_PARTICLES
#define MY_PARTICLES

#include "main.h"

#define MaxTrail 1000

//// CLASS PARTICLE

class myParticle
{
public:
	//Fields
	vec p;
	vec v;
	vec force;
	vec myTrail[MaxTrail];
	float mass;

	//Constructor
	myParticle(){
	
		p = vec();
		v = vec();
		force = vec();
		mass = 1;
	
	};

	//Overloaded COnstrcutor
	myParticle(vec _p, vec _v, vec _force, float _mass)
	{
		p = _p;
		v = _v;
		force = _force;
		mass = _mass;

	}

	// methods

	//gravity
	void applyGravity(vec &gravity, double &gravityStrength)
	{
		force += gravity * gravityStrength;

		//printf(" \n force11: %1.2f,  %1.2f, %1.2f ", particle.force.x, particle.force.y, particle.force.z);
	}

	// drag force
	void applyDragForce( double &dragStrength)
	{
		vec dragDir = v * -1;
		force += dragDir * dragStrength;
	}

	// PARTICLE to PARTICLE forces
	//Collision
	void collision( myParticle &particle2, double &collisionRadius, double &collisionStrength)
	{
		float distance = p.distanceTo(particle2.p);

		vec dir = p - particle2.p;

		vec collisionForce(0, 0, 0);
		if (distance < collisionRadius)
		{
			collisionForce = dir * -1;
			collisionForce.normalise();
		}

		force += collisionForce * collisionStrength;
		particle2.force += collisionForce * collisionStrength * -1;
		

	}

	// integrate Forces
	void integrateForces( double &timeStep)
	{
		vec a = force / mass; // acceleration = force /mass
		vec newVel = v + a *timeStep; // new vel = old vel + acc * timestep

		vec newPos = p + newVel * timeStep; // new pos =  old position + newVelocity*timestep

		// update values
		p = newPos;
		v = newVel;

		
		// Reset Forces
		force = vec(0, 0, 0);
	}



	//destructor
	~myParticle(){};
};



//// CLASS TARGET
class myTarget
{
public:
	// fields
	vec tarPos;
	double charge; // positive value  for attraction , negative for repulsion
	
	

	// constructor
	myTarget()
	{
		charge = 1;
	}

	// overloaded constructor
	myTarget(vec _tar)
	{

		tarPos = _tar;
	}


	//methods or functions

	void applyTargetForce(myParticle &particle, double &targetForceStrength)
	{

		float distance = particle.p.distanceTo(tarPos);

		vec dir = tarPos - particle.p;
		dir.normalise();

		particle.force += dir * targetForceStrength * charge;

		

	}

	void applyMagneticForce(myParticle &particle, double &magneticStrength)
	{

		float distance = particle.p.distanceTo(tarPos);

		vec dir = tarPos - particle.p;
		dir.normalise();

		
		vec f = (dir * magneticStrength * charge) / ((distance + 0.001) * (distance + 0.001));
		f.normalise();
		particle.force += f * 0.1;
		
	}

	// ADD other forces related to target here:

	// spin force
	void applySpinForce(myParticle &particle, double &spinStrength)
	{
		float distance = particle.p.distanceTo(tarPos);

		vec dir = tarPos - particle.p;
		dir.normalise();
		vec UpVec(0, 0, 1);
		vec spinDir = dir.cross(UpVec);

		particle.force += spinDir * spinStrength; // particles[i].force = particles[i].force + dir
		
	
	}



	//destructor
	~myTarget(){};
};



//// CLASS FORCE


#endif