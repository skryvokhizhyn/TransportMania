#ifndef _SIZE2D_H_
#define _SIZE2D_H_

#include "PointProxy.h"

namespace trm
{
	struct Size2d : PointProxy<size_t> 
	{
		Size2d() {};
		Size2d(const Type a, const Type b) : PointProxy(a, b) {}
		Size2d(const PointImpl & pImpl) : PointProxy(pImpl) {}
	};

} // namespace trm

#endif // _SIZE2D_H_