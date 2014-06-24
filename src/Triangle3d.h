#pragma once

#include "Point3d.h"
#include <utility>

namespace trm
{
	struct Triangle3d : PointImpl<Point3d, 3>
	{
		Triangle3d(){}
		Triangle3d(const PointImpl & pImpl) : PointImpl(pImpl) {}
		Triangle3d(const Type l, const Type e, const Type r) : PointImpl(l, e, r) {}
		Type & l() {return Get<0>();}
		const Type l() const {return Get<0>();}
		Type & e() {return Get<1>();}
		const Type e() const {return Get<1>();}
		Type & r() {return Get<2>();}
		const Type r() const {return Get<2>();}
	};
	
	// first is left, second is right
	typedef std::pair<Triangle3d, Triangle3d> Triangle3dPair;

} // namespace trm
