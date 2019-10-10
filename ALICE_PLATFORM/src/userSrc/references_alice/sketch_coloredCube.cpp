#define _MAIN_

#ifdef _MAIN_

#include "main.h"


#define N 5000 // number of points

vec pts[N]; // array to store points
float vals[N]; // array to store corresponding values
float minValue, maxValue; // variables to store min and max values of data ;

void setup()
{
	// randomly generate a point and its corresponding data value
	//store in corresponding arrays ;
	for (int i = 0; i < N; i++)
	{
		pts[i] = vec(ofRandom(-40, 40), ofRandom(-40, 40), ofRandom(0, 80));
		vals[i] = ofRandom(-10, 80);
	}
}

void update(int value)
{
	// --------------------------------------------- calculate min and max values in the data .
	// start with the assumption the minValue is very large , and maxvalue is very small.

	minValue = pow(10.0f, 10.0f); // 10 to the p	ower 10.
	maxValue = -1 * minValue; // negative 10 to the power 10 .

							  /* iterate through each value in the data array and
							  update  the minValue and maxValue is a lower or high value is found ;*/
	for (int i = 0; i < N; i++)
	{
		if (vals[i] < minValue)minValue = vals[i];
		if (vals[i] > maxValue)maxValue = vals[i];
	}
}

void draw()
{
	backGround(0.75);

	/*
	!!! IMPORTANT
	any set of drawing related code, that changes color , size and other display properties, should always be within
	a glPushAttrib(GL_CURRENT_BIT) & glPopAttrib() block . other changes to color and other properties will affect subsequent drawing commands.
	Example

	glPushAttrib(GL_CURRENT_BIT);
	// openGL drawing commands here ;
	glPopAttrib();

	*/

	glPushAttrib(GL_CURRENT_BIT);


	glEnable(GL_POINT_SMOOTH); // enable smooth display of points.

	for (int i = 0; i < N; i++)
	{
		vec4 clr = getColour(pts[i].z, minValue, maxValue); // convert a single float number to a color value between the min and max values.
		glColor3f(clr.r, clr.g, clr.b); // use that color, to set the color the next point that will be drawn

		float sz = ofMap(vals[i], minValue, maxValue, 1, 30); // remap the data range from minVlaue-maxValue range to 1-64 range.
		glPointSize(10); // use the remapped value to set point size
		drawPoint(pts[i]); // draw a point and at specific vector position ;
	}

	glPopAttrib();
}

void keyPress(unsigned char k, int xm, int ym)
{

}

void mousePress(int b, int s, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

#endif
