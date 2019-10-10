#include "main.h"
#include <array>
using namespace std;

class Snake
{
public:
	Snake();
	~Snake();
	void update(double _playground_x, double _playground_y, double _scl);
	void draw();
	void dir(double x, double y);
	bool eat(vec _food);
	void death();

	double x = 0;
	double y = 0;
	double x_speed = 0;
	double y_speed = 0;
	double n_tail;
	double tail_x[100], tail_y[100];
	double scl; // scale of the gird
	double playground_x;
	double playground_y;

private:

};


Snake::Snake()
{
}

void Snake::update(double _playground_x, double _playground_y, double _scl)
{
	scl = _scl;
	playground_x = _playground_x;
	playground_y = _playground_y;

	double prev_x = tail_x[0];
	double prev_y = tail_y[0];
	double prev2_x, prev2_y;
	tail_x[0] = x;
	tail_y[0] = y;
	for (int i = 1; i < n_tail; i++)
	{
		prev2_x = tail_x[i];
		prev2_y = tail_y[i];
		tail_x[i] = prev_x;
		tail_y[i] = prev_y;
		prev_x = prev2_x;
		prev_y = prev2_y;
	}

	x = x + x_speed * scl;
	y = y + y_speed * scl;

	if (x == -(playground_x / 2) || x == (playground_x / 2)) x_speed = 0;
	if (y == -(playground_y / 2) || y == (playground_y / 2)) y_speed = 0;
}

void Snake::draw()
{
	// drawwing the head
	glPushMatrix();
	glTranslated(x, y, 0);
	drawRectangle(vec(-scl / 2, -scl / 2, 0), vec(scl / 2, scl / 2, 0));
	glPopMatrix();



	// drawing the tail
	for (int i = 0; i < n_tail; i++)
	{
		glPushMatrix();
		glTranslated(tail_x[i], tail_y[i], 0);
		drawRectangle(vec(-scl / 2, -scl / 2, 0), vec(scl / 2, scl / 2, 0));
		glPopMatrix();
	}
}

void Snake::dir(double x, double y)
{
	x_speed = x;
	y_speed = y;
}

bool Snake::eat(vec _food)
{
	vec food = _food;
	vec s_location = vec(x, y, 0);
	double d = s_location.distanceTo(food);

	if (d < scl)
	{
		n_tail++;
		return true;
	}
	else
		return false;
}

void Snake::death()
{
	for (int i = 0; i < n_tail; i++)
	{
		vec pos_tail = vec(tail_x[i], tail_y[i], 0);
		vec pos_head = vec(x, y, 0);
		double d = pos_tail.distanceTo(pos_head);
		if (d < 1)
		{
			x_speed = 0;
			y_speed = 0;
		}
	}
}

Snake::~Snake()
{
}
