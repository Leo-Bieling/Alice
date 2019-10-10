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

	//given 4 points in local space
	vec A, B, C, D;
	A = vec(-1, -1, 0);
	B = vec(-1, 1, 0);
	C = vec(1, 1, 0);
	D = vec(1, -1, 0);

	//given u, v, n, basis vector of tranformes space
	vec u(1, 1, 0);
	vec v = u.cross(vec(0, 0, 1));
	vec n = u.cross(v);
	u.normalise();
	v.normalise();
	n.normalise();

	//draw square
	drawLine(A, B);
	drawLine(B, C);
	drawLine(C, D);
	drawLine(D, A);

	//construct the matrix, by populating the matrix with u,v,n vectors;
	Matrix3 rotMatrix; //creates empty matrix
	rotMatrix.setColumn(0, u); //puts u values to 0 column
	rotMatrix.setColumn(1, v);
	rotMatrix.setColumn(2, n);

	//get new points -> Pnew = M * Pold
	vec Anew = rotMatrix * A;
	vec Bnew = rotMatrix * B;
	vec Cnew = rotMatrix * C;
	vec Dnew = rotMatrix * D;

	//draw new points as lines
	glColor3f(1, 0, 0);
	drawLine(Anew, Bnew);
	drawLine(Bnew, Cnew);
	drawLine(Cnew, Dnew);
	drawLine(Dnew, Anew);
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