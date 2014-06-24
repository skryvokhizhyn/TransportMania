#ifndef _POINT3D_H_
#define _POINT3D_H_

#include "PointImpl.h"

namespace trm
{
	struct Point3d : PointImpl<float, 3>
	{
		Point3d(){}
		Point3d(const PointImpl & pImpl) : PointImpl(pImpl) {}
		Point3d(const Type a, const Type b, const Type c) : PointImpl(a, b, c) {}
		Type & x() {return Get<0>();}
		const Type x() const {return Get<0>();}
		Type & y() {return Get<1>();}
		const Type y() const {return Get<1>();}
		Type & z() {return Get<2>();}
		const Type z() const {return Get<2>();}
	};

} // namespace trm

#endif // _POINT3D_H_