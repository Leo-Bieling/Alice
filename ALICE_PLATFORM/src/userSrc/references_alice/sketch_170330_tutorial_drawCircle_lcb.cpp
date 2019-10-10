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


void drawCircleDivision(vec A, vec B, float r)
{
		vec A_B;
		A_B = A + B;
		A_B.normalise();
		A_B *= r;
		drawPoint(A_B);

		vec A_AB;
		A_AB = A + A_B;
		A_AB.normalise();
		A_AB *= r;
		drawPoint(A_AB);

		vec AB_B;
		AB_B = A_B + B;
		AB_B.normalise();
		AB_B *= r;
		drawPoint(AB_B);

		vec A_AAB;
		A_AAB = A + A_AB;
		A_AAB.normalise();
		A_AAB *= r;
		drawPoint(A_AAB);

		vec AB_ABB;
		AB_ABB = A_B + AB_B;
		AB_ABB.normalise();
		AB_ABB *= r;
		drawPoint(AB_ABB);

		vec AB_AAB;
		AB_AAB = A_B + A_AB;
		AB_AAB.normalise();
		AB_AAB *= r;
		drawPoint(AB_AAB);

		vec ABB_B;
		ABB_B = AB_B + B;
		ABB_B.normalise();
		ABB_B *= r;
		drawPoint(ABB_B);

		drawPoint(A);
		drawPoint(B);
}

///////// ----------------------------------------- model - view - controller (MVC) paradigm / pattern / template  ----------------- ////////////////////////////// 
/////////////////////////// model  ///////////////////////////////////////////
void setup()
{

}

void update(int value)
{

}

/////////////////////////// view  ///////////////////////////////////////////
void draw()
{
		backGround(0.75);
		drawGrid(20);

		float r = 5;
		vec A(r, 0, 0);
		vec B(0, r, 0);

		glColor3f(1, .5, 1);
		drawCircleDivision(A, B, r);
		drawCircleDivision(B, A*-1, r);
		drawCircleDivision(A*-1, B*-1, r);
		drawCircleDivision(B*-1, A, r);

		vec u, v, n, o, newO;
		newO = vec(0, 5, 20);
		o = vec(0, 0, 0);
		u = vec(5, 3, 7);
		v = u.cross(vec(0, 1, 0));
		n = u.cross(v);

		u.normalise();
		v.normalise();
		n.normalise();

		glColor3f(1, 0, 0);
		drawLine(o + newO, u + newO);
		glColor3f(0, 1, 0);
		drawLine(o + newO, v + newO);
		glColor3f(0, 0, 1);
		drawLine(o + newO, n + newO);

		Matrix3 rotMatrix;
		rotMatrix.setColumn(0, u);
		rotMatrix.setColumn(1, v);
		rotMatrix.setColumn(2, n);

		
		glPushMatrix();
		glTranslatef(0, 0, 10);
		glRotatef(30, 1, 1, 0);
		glColor3f(1, 0, 1);
		drawCircleDivision(A, B, r);
		drawCircleDivision(B, A*-1, r);
		drawCircleDivision(A*-1, B*-1, r);
		drawCircleDivision(B*-1, A, r);
		glPopMatrix();
}

/////////////////////////// control  ///////////////////////////////////////////
void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

void keyPress(unsigned char k, int xm, int ym)
{

}
#endif // _MAIN_