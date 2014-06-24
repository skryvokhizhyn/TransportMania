#ifndef _POINT2D_H_
#define _POINT2D_H_

#include "Types.h"
#include "PointProxy.h"

namespace trm
{
	struct Point2d : PointProxy<AxisType> 
	{
		Point2d() {}
		Point2d(const Type a, const Type b) : PointProxy(a, b) {}
		Point2d(const PointImpl & pImpl) : PointProxy(pImpl) {}
	};

} // namespace trm

#endif // _POINT2D_H_