//#define _MAIN_

#ifdef _MAIN_

#include <iostream>
#include <vector>
#include <tuple>
#include <numeric>
#include <cmath>
#include <limits>

#include "main.h"

#include <headers/include/zCore.h>
#include <headers/include/zData.h>
#include <headers/include/zDisplay.h>
#include <headers/include/zGeometry.h>
#include <headers/include/zIO.h>


using namespace zSpace;
using namespace std;


class LinearRegression
{
public:

	LinearRegression() {}
	~LinearRegression() {}
	LinearRegression(vector<double> & m_x_vals_, vector<double> m_y_vals_) : m_x_vals(m_x_vals_), m_y_vals(m_y_vals_), m_num_elems(m_y_vals_.size()), m_old_err(std::numeric_limits<double>::max()) {}

	void trainAlgorithm(int num_iters_, double a_init_, double b_init_)
	{
		int iter = 0;
		m_a = a_init_;
		m_b = b_init_;

		while (!isConverged() && iter < num_iters_)
		{
			// update the gradient
			double step = 2 / double(iter + 2); //determines how fast we converge to a solution
			double a_grad = 0;
			double b_grad = 0;

			// compute the gradient of error wrt to a
			for (int i = 0; i < m_num_elems; i++)
			{
				a_grad += m_x_vals[i] * ((m_a * m_x_vals[i] + m_b) - m_y_vals[i]);
			}
			a_grad = (2 * a_grad) / m_num_elems;

			// coompute the gradient of error wrt to b
			for (int i = 0; i < m_num_elems; i++)
			{
				b_grad += ((m_a * m_x_vals[i] + m_b) - m_y_vals[i]);
			}
			b_grad = (2 * b_grad) / m_num_elems;

			// take a step
			m_a = m_a - (step * a_grad);
			m_b = m_b - (step * b_grad);

			// output to the consol
			cout << "a:\t" << m_a << ", b:\t" << m_b << "\r\n";
			cout << "grad_a:\t" << a_grad << ", grad_b:\t" << b_grad << "\r\n";

			// iterate the while loop
			iter++;
		}
	}
	double regress(double x_)
	{
		double res = m_a * x_ + m_b;
		return res;
	}

private:

	bool isConverged()
	{
		double error = 0;
		double thresh = 0.001;
		for (int i = 0; i < m_num_elems; i++)
		{
			error += ((m_a * m_x_vals[i] + m_b) - m_y_vals[i]) * ((m_a * m_x_vals[i] + m_b) - m_y_vals[i]);
		}
		error /= m_num_elems;
		cout << "Error: " << error << "\r\n";
		bool res = (abs(error) > m_old_err - thresh && abs(error) < m_old_err + thresh) ? true : false;
		m_old_err = abs(error);
		return res;
	}

	vector<double> m_x_vals;
	vector<double> m_y_vals;
	double m_num_elems;
	double m_old_err;
	double m_a;
	double m_b;
};

vector<double> y({ 2.8, 2.9, 7.6, 9, 8.6, 10.5 });
vector<double> x({ 1, 2, 3, 4, 5, 6 });
LinearRegression lr(x, y);


////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	lr.trainAlgorithm(1000, 3, -10);

	cout << lr.regress(3) << endl;
}

void update(int value)
{

}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	backGround(0.75);
	drawGrid(20.0);

	for (int i = 0; i < x.size(); i++)
	{
		drawPoint(zVector(x[i], y[i], 0), zColor(0,0,0,1), 4.0);
	}

	drawLine(zVector(0, lr.regress(0), 0), zVector(10, lr.regress(10), 0), zColor(1, 0, 0, 1), 2.0);

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

#endif _MAIN_