#pragma warning(push)
#pragma warning(disable: 4100) // 'rank' : unreferenced formal parameter

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

#pragma warning(pop)