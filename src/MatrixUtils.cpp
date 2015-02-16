#pragma warning(disable: 4100) // 'rank' : unreferenced formal parameter
#pragma warning(disable: 4127) // conditional expression is constant

#include "MatrixUtils.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

using namespace trm;
using namespace boost::numeric::ublas;

Matrix trm::operator * (const Matrix & m1, const Matrix & m2)
{
	return prod(m1, m2);
}

Point4d trm::operator * (const Matrix & m, const Point4d & v)
{
	Point4d res;

	const size_t rows = m.size1();
	for (size_t i = 0; i < rows; ++i)
	{
		const matrix_row<const Matrix> mr (m, i);
		res[i] = std::inner_product(mr.begin(), mr.end(), v.Begin(), 0.0f);
	}

	return res;
}

Point3d trm::operator * (const Matrix & m, const Point3d & v)
{
	Point4d p4d = Point4d::Cast(v);
	p4d.w() = 1.0f;

	p4d = m * p4d;
	p4d /= p4d.w();

	return Point3d::Cast(p4d);
}

//const Point4d trm::operator * (const Point4d & v, const Matrix & m)
//{
//	Point4d res;
//
//	const size_t cols = m.size2();
//	for (size_t j = 0; j < cols; ++j)
//	{
//		const matrix_column<const Matrix> mr (m, j);
//		res[j] = std::inner_product(mr.begin(), mr.end(), v.Begin(), 0.0f);
//	}
//
//	return res;
//}

Matrix trm::Transponate(const Matrix & m)
{
	return trans(m);
}

#include <boost/numeric/ublas/lu.hpp>
#include "MatrixFactory.h"

Matrix trm::Inverse(const Matrix & m)
{
	typedef permutation_matrix<std::size_t> pmatrix;

	// create a permutation matrix for the LU-factorization
	pmatrix pm(m.size1());

	Matrix cp = m;

	// perform LU-factorization
	int res = lu_factorize(cp, pm);
	if (res != 0)
	{
		throw std::runtime_error("Cannot inverse matrix");
	}

	Matrix inverse = MatrixFactory::Identity();

	// backsubstitute to get the inverse
	lu_substitute(cp, pm, inverse);

	return inverse;
}
