#ifndef _MYMATRIX_H_
#define _MYMATRIX_H_

#include "Types.h"
#include <boost/numeric/ublas/matrix.hpp>

namespace trm
{
	typedef boost::numeric::ublas::c_matrix<AxisType, 4, 4> Matrix;
} // namespace trm

#endif // _MYMATRIX_H_