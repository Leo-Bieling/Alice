
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



// Note that the functions are all inline and simple, so the compiler
// should completely optimize them away.
template<typename MatrixType> struct matrix_traits
{
	typedef typename MatrixType::index_type index_type;
	typedef typename MatrixType::value_type value_type;
	static index_type min_row(MatrixType const& A)
	{
		return A.min_row();
	}
	static index_type max_row(MatrixType const& A)
	{
		return A.max_row();
	}
	static index_type min_column(MatrixType const& A)
	{
		return A.min_column();
	}
	static index_type max_column(MatrixType const& A)
	{
		return A.max_column();
	}
	static value_type& element(MatrixType& A, index_type i, index_type k)
	{
		return A(i, k);
	}
	static value_type element(MatrixType const& A, index_type i, index_type k)
	{
		return A(i, k);
	}
};

// specialization of the matrix traits for built-in two-dimensional
// arrays
template<typename T, std::size_t rows, std::size_t columns>
struct matrix_traits<T[rows][columns]>
{
	typedef std::size_t index_type;
	typedef T value_type;
	static index_type min_row(T const (&)[rows][columns])
	{
		return 0;
	}
	static index_type max_row(T const (&)[rows][columns])
	{
		return rows - 1;
	}
	static index_type min_column(T const (&)[rows][columns])
	{
		return 0;
	}
	static index_type max_column(T const (&)[rows][columns])
	{
		return columns - 1;
	}
	static value_type& element(T(&A)[rows][columns],
		index_type i, index_type k)
	{
		return A[i][k];
	}
	static value_type element(T const (&A)[rows][columns],
		index_type i, index_type k)
	{
		return A[i][k];
	}
};

// Swap rows i and k of a matrix A
// Note that due to the reference, both dimensions are preserved for
// built-in arrays
template<typename MatrixType>
void swap_rows(MatrixType& A,
	typename matrix_traits<MatrixType>::index_type i,
	typename matrix_traits<MatrixType>::index_type k)
{
	matrix_traits<MatrixType> mt;
	typedef typename matrix_traits<MatrixType>::index_type index_type;

	// check indices
	assert(mt.min_row(A) <= i);
	assert(i <= mt.max_row(A));

	assert(mt.min_row(A) <= k);
	assert(k <= mt.max_row(A));

	for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
		std::swap(mt.element(A, i, col), mt.element(A, k, col));
}

// divide row i of matrix A by v
template<typename MatrixType>
void divide_row(MatrixType& A,
	typename matrix_traits<MatrixType>::index_type i,
	typename matrix_traits<MatrixType>::value_type v)
{
	matrix_traits<MatrixType> mt;
	typedef typename matrix_traits<MatrixType>::index_type index_type;

	assert(mt.min_row(A) <= i);
	assert(i <= mt.max_row(A));

	assert(v != 0);

	for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
		mt.element(A, i, col) /= v;
}

// in matrix A, add v times row k to row i
template<typename MatrixType>
void add_multiple_row(MatrixType& A,
	typename matrix_traits<MatrixType>::index_type i,
	typename matrix_traits<MatrixType>::index_type k,
	typename matrix_traits<MatrixType>::value_type v)
{
	matrix_traits<MatrixType> mt;
	typedef typename matrix_traits<MatrixType>::index_type index_type;

	assert(mt.min_row(A) <= i);
	assert(i <= mt.max_row(A));

	assert(mt.min_row(A) <= k);
	assert(k <= mt.max_row(A));

	for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
		mt.element(A, i, col) += v * mt.element(A, k, col);
}

// convert A to reduced row echelon form
template<typename MatrixType>
void to_reduced_row_echelon_form(MatrixType& A)
{
	matrix_traits<MatrixType> mt;
	typedef typename matrix_traits<MatrixType>::index_type index_type;

	index_type lead = mt.min_row(A);

	for (index_type row = mt.min_row(A); row <= mt.max_row(A); ++row)
	{
		if (lead > mt.max_column(A))
			return;
		index_type i = row;
		while (mt.element(A, i, lead) == 0)
		{
			++i;
			if (i > mt.max_row(A))
			{
				i = row;
				++lead;
				if (lead > mt.max_column(A))
					return;
			}
		}
		swap_rows(A, i, row);
		divide_row(A, row, mt.element(A, row, lead));
		for (i = mt.min_row(A); i <= mt.max_row(A); ++i)
		{
			if (i != row)
				add_multiple_row(A, i, row, -mt.element(A, i, lead));
		}
	}
}


////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	//float B[12][6] = {	{ -(1.0 / (2 * sqrtf(3.0))), sqrtf(3.0) / 2, -(1 / sqrtf(3.0)), 0, 0, 0 },
	//					{ 0, 0, -(sqrtf(3.0) / 2), 0, 0, sqrtf(3.0) / 2 },
	//					{ 0, 0, 1.0 / sqrtf(3.0), 0, 1.0 / (2 * sqrtf(3.0)), -(sqrtf(3.0) / 2) },
	//					{ 1.0 / (2 * sqrtf(3.0)), 0, 0, 0, -(1.0 / (2 * sqrtf(3.0))), 0 },
	//					{ -0.5, 0.5, 0, 0, 0, 0 },
	//					{ 0, -0.5, 0, 1, 0, -0.5 },
	//					{ 0, 0, 0, 0, -0.5, 0.5 },
	//					{ 0.5, 0, 0, -1, 0.5, 0 },
	//					{ sqrtf(2.0) / sqrtf(3.0), 0, -(sqrtf(2.0) / sqrtf(3.0)), 0, 0, 0 },
	//					{ 0, 0, 0, 0, 0, 0 },
	//					{ 0, 0, sqrtf(2.0) / sqrtf(3.0), 0,  -(sqrtf(2.0) / sqrtf(3.0)), 0 },
	//					{ -(sqrtf(2.0) / sqrtf(3.0)), 0, 0, 0, sqrtf(2.0) / sqrtf(3.0), 0 } };

	double M[12][6] = { { -0.288675, 0.866025, -0.57735, 0, 0, 0 },
						{ 0, 0, -0.866025, 0, 0, 0.866025 },
						{ 0, 0, 0.57735, 0, 0.288675, -0.866025 },
						{ 0.288675, 0, 0, 0, -0.288675, 0 },
						{ -0.5,	0.5, 0,	0, 0, 0 },
						{ 0, -0.5, 0, 1, 0, -0.5},
						{ 0, 0,	0, 0, -0.5, 0.5 },
						{ 0.5,	0, 0, -1, 0.5, 0 },
						{ 0.816497,	0, -0.816497, 0, 0,	0 },
						{ 0, 0,	0, 0, 0, 0 },
						{ 0, 0,	0.816497, 0, -0.816497, 0 },
						{ -0.816497, 0,	0, 0, 0.816497,	0 } 
	};

	int rows_count = extent<decltype(M), 0>::value;
	int cols_count = extent<decltype(M), 1>::value;

	to_reduced_row_echelon_form(M);

	for (int i = 0; i < rows_count; ++i)
	{
		for (int j = 0; j < cols_count; ++j)
			cout << setw(7) << M[i][j] << '\t';
		cout << "\n";
	}
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