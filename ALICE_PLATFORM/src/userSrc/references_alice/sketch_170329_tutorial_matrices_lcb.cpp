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

vec u(1, 0, 0);

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

		vec O(0, 0, 0);
		vec newO(10,0,0);

		//given u, v, n, basis vector of tranformes space
		vec v = u.cross(vec(0, 0, 1));
		vec n = u.cross(v);
		u.normalise();
		v.normalise();
		n.normalise();

		glColor3f(1, 0, 0);
		drawLine(O , u * 2);
		glColor3f(0, 1, 0);
		drawLine(O , v * 2);
		glColor3f(0, 0, 1);
		drawLine(O, n * 2);

		//construct the matrix for rotation
		Matrix4 rotMatrix;
		rotMatrix.setColumn(0, u);
		rotMatrix.setColumn(1, v);
		rotMatrix.setColumn(2, n);

		//rotate first cube
		A = rotMatrix * A;
		B = rotMatrix * B;
		C = rotMatrix * C;
		D = rotMatrix * D;
		E = rotMatrix * E;
		F = rotMatrix * F;
		G = rotMatrix * G;
		H = rotMatrix * H;

		//draw first cube
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

		//construct the matrix for move
		Matrix4 moveMatrix;
		moveMatrix.setColumn(3, newO);
	
		//rotate and move second cube
		vec Anew = rotMatrix * moveMatrix * A;
		vec Bnew = rotMatrix * moveMatrix * B;
		vec Cnew = rotMatrix * moveMatrix * C;
		vec Dnew = rotMatrix * moveMatrix * D;
		vec Enew = rotMatrix * moveMatrix * E;
		vec Fnew = rotMatrix * moveMatrix * F;
		vec Gnew = rotMatrix * moveMatrix * G;
		vec Hnew = rotMatrix * moveMatrix * H;

		//draw second cube
		glColor3f(1, 0, 1);
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
	if (k == 'p')
	{
		u.x += u.y * 0.05;
		u.y += u.x * -0.05;
	}
}
#endif // _MAIN_



