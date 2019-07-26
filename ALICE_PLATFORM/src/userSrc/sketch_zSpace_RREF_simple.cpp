
//#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v002
// zSpace header
#include <headers/app/include/zObjects.h>
#include <headers/app/include/zFnSets.h>
#include <headers/app/include/zViewer.h>

// include toolset header
#include <headers/app/include/Tools/zTsGeometry.h>

using namespace zSpace;
using namespace std;


void print_matrix(float A[][6]);
void row_reduce(float A[][6]);

void print_matrix(float A[][6])
{
	int p = 12;
	int q = 6;

	for (int i = 0; i < p; i++) {
		for (int j = 0; j < q; j++) {
			cout << setw(7) << setprecision(2) << A[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
}

void row_reduce(float A[][6])
{
	const int number_of_rows = 12;
	const int number_of_cols = 6;

	int lead = 0;

	while (lead < number_of_rows) {
		float d, m;

		for (int r = 0; r < number_of_rows; r++) { // for each row ...
			/* calculate divisor and multiplier */
			d = A[lead][lead];
			m = A[r][lead] / A[lead][lead];

			for (int c = 0; c < number_of_cols; c++) { // for each column ...
				if (r == lead)
					A[r][c] /= d;               // make pivot = 1
				else
					A[r][c] -= A[lead][c] * m;  // make other = 0
			}
		}

		lead++;
		print_matrix(A);
	}
}

////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	float A[3][4] = { {5, -6, -7,   7},
					 {3, -2,  5, -17},
					 {2,  4, -3,  29} }; //answer should be {2, 4, -3}

	float B[12][6] = {	{-(1 / (2 * sqrt(3))), sqrt(3) / 2, -(1 / sqrt(3)), 0, 0, 0},
						{0, 0, -(sqrt(3) / 2), 0, 0, sqrt(3) / 2},
						{0, 0, 1 / sqrt(3), 0, 1 / (2 * sqrt(3)), -(sqrt(3) / 2)},
						{1 / (2 * sqrt(3)), 0, 0, 0, -(1 / (2 * sqrt(3))), 0},
						{-0.5, 0.5, 0, 0, 0, 0},
						{0, -0.5, 0, 1, -0.5},
						{0, 0, 0, 0, -0.5, 0.5},
						{0.5, 0, 0, -1, 0.5, 0},
						{sqrt(2) / sqrt(3), 0, -(sqrt(2) / sqrt(3)), 0, 0, 0,},
						{0, 0, 0, 0, 0, 0},
						{0, 0, sqrt(2) / sqrt(3), 0,  -(sqrt(2) / sqrt(3)), 0},
						{-(sqrt(2) / sqrt(3)), 0, 0, 0, sqrt(2) / sqrt(3), 0} };

	float M[12][6] = { {  -0.288675,	0.866025, -0.57735,	0,	0,	0 },
			 {  0,	0, -0.866025,	0,	0,	0.866025 },
			 { 0,	0,	0.57735,	0,	0.288675, -0.866025 } ,
			{0.288675,	0,	0,	0, -0.288675,	0 } ,
			{ -0.5,	0.5,	0,	0,	0,	0 } ,
			{ 0, -0.5,	0,	1,	0, -0.5} ,
			{ 0,	0,	0,	0, -0.5, 0.5 } ,
			{ 0.5,	0,	0, -1,	0.5,	0} ,
			{ 0.816497,	0, -0.816497,	0,	0,	0 } ,
			{ 0,	0,	0,	0,	0, 0 } ,
			{ 0,	0,	0.816497,	0 , -0.816497,	0 } ,
			{ -0.816497,	0,	0,	0,	0.816497,	0 } };

	print_matrix(M);
	row_reduce(M);
}

void update(int value)
{

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	//drawGrid(20);
	backGround(0.75);
}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{
}

void mousePress(int b, int state, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif // _MAIN_