#pragma once

#include "PointProxy.h"

namespace trm
{
	struct Point2i : PointProxy<int> 
	{
		Point2i() {}
		Point2i(const Type a, const Type b) : PointProxy(a, b) {}
		Point2i(const PointImpl & pImpl) : PointProxy(pImpl) {}
	};

} // namespace trm
