#ifndef _MATRIXUTILS_H_
#define _MATRIXUTILS_H_

#include "Matrix.h"
#include "Point4d.h"

namespace trm
{
	const Matrix operator * (const Matrix & m1, const Matrix & m2);
	const Point4d operator * (const Matrix & m, const Point4d & v);
	const Matrix Transponate(const Matrix & m);

} // namespace trm

#endif // _MATRIXUTILS_H_