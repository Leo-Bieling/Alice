#pragma once

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zData.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zIO.h>

using namespace zSpace;
using namespace std;

class Boid
{
public:
	Boid();
	~Boid();
	Boid(zVector _position);

	void flock(vector<Boid> _boids);
	void update(vector<Boid> _boids);
	zVector seek(zVector _target);
	void draw();
	void borders();
	zVector seperate(vector<Boid> _boids);
	zVector align(vector<Boid> _boids);
	zVector cohesion(vector<Boid> _boids);

	zVector position;
	zVector velocity;
	zVector acceleration;
	float r;
	float maxforce; //maxiumum steering force
	float maxspeed; //maximum speed

private:

};

Boid::Boid()
{
}

Boid::~Boid()
{
}

Boid::Boid(zVector _position)
{
	//acceleration = zVector(0, 0, 0);
	
	double angle = randomNumber_double(0, TWO_PI);
	velocity = zVector(cos(angle), sin(angle), 0); // change this for 3d

	position = _position;
	//r = 2.0; // not necessary with only dots
	//maxspeed = 1.0;
	//maxforce = 0.03;
}

void Boid::flock(vector<Boid> _boids)   // accumulate a new acceleration each time based on three rules
{
	zVector sep = seperate(_boids);	// seperation
	zVector ali = align(_boids);		// alignment
	zVector coh = cohesion(_boids);	// cohension
	
	// arbitrarily weight of the three forces
	sep *= 1.5;
	ali *= 1.0;
	coh *= 1.0;
	
	//adding the force vectors to acceleration
	acceleration += sep;
	acceleration += ali;
	acceleration += coh;
}

void Boid::update(vector<Boid> _boids)
{
	//velocity += acceleration;	// update velocity
	//velocity.normalize();		// limit speed IS MISSING ////////////////////////////////////////
	//velocity *= maxspeed;

	position += velocity;
	//acceleration *= 0;			// reset acceleration to 0 each cycle
	cout << "p.x= " << position.x << " - p.y= " << position.y << endl;
	cout << "vel.x= " << velocity.x << " - vel.y= " << velocity.y << endl;

	//flock(_boids);
	//borders();
}

zVector Boid::seek(zVector _target)  // a method that calculates and applies a steering force towards a target
{
	zVector desired = (_target - position); // a vector pointing from the position to the target
	// scale to maximum speed
	desired.normalize();
	desired *= maxspeed;

	zVector steer = desired - velocity;
	steer.normalize();	// limit the maximum steering force //////////////////////
	steer *= maxforce;
	return steer;
}

void Boid::draw()
{
	drawPoint(position, zColor(1, 0, 0, 1), 10.0); // for now just a point - later triangle
}

void Boid::borders()
{
	int width = 40;		// x
	int depth = 40;		// y
	int height = 40;	// z

	// wraparoung
	if (position.x > width / 2) position.x = -width / 2;
	if (position.x < -width / 2) position.x = width / 2;
	if (position.y > depth / 2) position.y = -depth / 2;
	if (position.y < -depth / 2) position.y = depth / 2;
	if (position.z > height / 2) position.z = -height / 2;
	if (position.z < -height / 2) position.z = height / 2;

}

zVector Boid::seperate(vector<Boid> _boids) // function checks for near by boids and steers away
{
	float desiredSeperation = 2.0;
	zVector steer = zVector(0, 0, 0);
	int count = 0;
	// for every boid in the system, check if it to close
	for(Boid other : _boids)
	{
		float d = position.distanceTo(other.position);
		if ((d > 0) && (d < desiredSeperation))
		{
			// calculate vector pointing away from neighor
			zVector diff = position - other.position;
			diff.normalize();
			diff /= d; // weight by distance
			steer += diff;
			count++; // keep track of how many
		}
	}

	// average - devide by how many
	if (count > 0) steer /= count;

	// as long as the vector is greater than 0
	if (steer.length() > 0)
	{
		steer.normalize();
		steer *= maxspeed;
		steer -= velocity;
		// steer.limit is missing ///////////////////////////////////////////
	}

	return steer;
}

zVector Boid::align(vector<Boid> _boids) // for every near by boid in the system, calculate the average velocity
{
	float neighborDist = 4.0;
	zVector sum = zVector(0, 0, 0);
	int count = 0;

	for (Boid other : _boids)
	{
		float d = position.distanceTo(other.position);
		if ((d > 0) && (d < neighborDist))
		{
			sum += other.velocity;
			count++;
		}
	}

	if (count > 0)
	{
		sum /= count;
		sum.normalize();
		sum *= maxspeed;
		zVector steer = sum - velocity;
		// steet.limit is missing ////////////////////////////
		return steer;
	}
	else
	{
		return zVector(0, 0, 0);
	}
}

zVector Boid::cohesion(vector<Boid> _boids) // for the average position off all nearbt boids, calculate steering vector towards that position
{
	float neighborDist = 4.0;
	zVector sum = zVector(0, 0, 0); // start with empty vector to accumulate all positions
	int count = 0;
	for (Boid other : _boids)
	{
		float d = position.distanceTo(other.position);
		if ((d > 0) && (d < neighborDist))
		{
			sum += other.position;
			count++;
		}
	}

	if (count > 0)
	{
		sum /= count;
		return seek(sum); // steer towards the position
	}
	else
	{
		return zVector(0, 0, 0);
	}
}