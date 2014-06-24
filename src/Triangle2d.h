#pragma once

#include "PointImpl.h"
#include "Point2d.h"

namespace trm
{
	struct Triangle2d : PointImpl<Point2d, 3>
	{
		Triangle2d(){}
		Triangle2d(const PointImpl & pImpl) : PointImpl(pImpl) {}
		Triangle2d(const Type l, const Type e, const Type r) : PointImpl(l, e, r) {}
		Type & l() {return Get<0>();}
		const Type l() const {return Get<0>();}
		Type & e() {return Get<1>();}
		const Type e() const {return Get<1>();}
		Type & r() {return Get<2>();}
		const Type r() const {return Get<2>();}
	};

	// first is left, second is right
	typedef std::pair<Triangle2d, Triangle2d> Triangle2dPair;

} // namespace trm