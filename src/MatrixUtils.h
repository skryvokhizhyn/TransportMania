#ifndef _MATRIXUTILS_H_
#define _MATRIXUTILS_H_

#include "Matrix.h"
#include "Point4d.h"

namespace trm
{
	Matrix operator * (const Matrix & m1, const Matrix & m2);
	Point4d operator * (const Matrix & m, const Point4d & v);
	Point3d operator * (const Matrix & m, const Point3d & v);
	Matrix Transponate(const Matrix & m);
	Matrix Inverse(const Matrix & m);

} // namespace trm

#endif // _MATRIXUTILS_H_