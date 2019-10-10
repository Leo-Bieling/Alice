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

	vec O(0, 0, 0);
	//given 8 points in local space
	vec A, B, C, D, E, F, G, H;
	A = vec(-1, -1, -1);
	B = vec(-1, 1, -1);
	C = vec(1, 1, -1);
	D = vec(1, -1, -1);
	E = vec(-1, -1, 1);
	F = vec(-1, 1, 1);
	G = vec(1, 1, 1);
	H = vec(1, -1, 1);


	//given u, v, n, basis vector of tranformes space
	vec u(1, 1, 1);
	vec v = u.cross(vec(0, 0, 1));
	vec n = u.cross(v);
	u.normalise();
	v.normalise();
	n.normalise();

	glColor3f(1, 0, 0);
	drawLine(O, u);
	glColor3f(0, 1, 0);
	drawLine(O, v); 
	glColor3f(0, 0, 1);
	drawLine(O, n);

	//draw square
	glColor3f(0, 0, 0);
	drawLine(A, B);
	drawLine(B, C);
	drawLine(C, D);
	drawLine(D, A);
	drawLine(E, F);
	drawLine(F, G);
	drawLine(G, H);
	drawLine(H, E);
	drawLine(A, E);
	drawLine(B, F);
	drawLine(C, G);
	drawLine(D, H);

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
	vec Enew = rotMatrix * E;
	vec Fnew = rotMatrix * F;
	vec Gnew = rotMatrix * G;
	vec Hnew = rotMatrix * H;

	//draw new points as lines
	glColor3f(1, 0, 0);
	drawLine(Anew, Bnew);
	drawLine(Bnew, Cnew);
	drawLine(Cnew, Dnew);
	drawLine(Dnew, Anew);
	drawLine(Enew, Fnew);
	drawLine(Fnew, Gnew);
	drawLine(Gnew, Hnew);
	drawLine(Hnew, Enew);
	drawLine(Anew, Enew);
	drawLine(Bnew, Fnew);
	drawLine(Cnew, Gnew);
	drawLine(Dnew, Hnew);
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