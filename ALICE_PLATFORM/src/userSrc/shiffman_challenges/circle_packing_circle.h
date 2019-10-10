#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zData.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zIO.h>


using namespace zSpace;
using namespace std;

class Circle
{
public:
	Circle();
	Circle(zVector _position);
	~Circle();
	void draw();
	void grow();
	bool edges();

	zVector position;
	float radius;
	bool growing = true;

private:

};

Circle::Circle()
{
}

Circle::Circle(zVector _position)
{
	position = _position;
	radius = 0.0;
}

Circle::~Circle()
{
}

void Circle::draw()
{
	drawCircle(position, 32, radius, true, zColor(0,0,0,1), 2);
}

void Circle::grow()
{
	if (growing)
	{
		radius += 0.01;
	}
}

bool Circle::edges()
{
	return ((position.x + radius) > 40 || (position.x - radius) < -40 || (position.y + radius) > 40 || (position.y - radius) < -40);
}