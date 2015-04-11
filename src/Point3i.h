#pragma once

#include "PointImpl.h"

namespace trm
{
	struct Point3i : PointImpl<int, 3>
	{
		Point3i(){}
		Point3i(const PointImpl & pImpl) : PointImpl(pImpl) {}
		Point3i(const Type a, const Type b, const Type c) : PointImpl(a, b, c) {}
		Type & x() {return Get<0>();}
		const Type x() const {return Get<0>();}
		Type & y() {return Get<1>();}
		const Type y() const {return Get<1>();}
		Type & z() {return Get<2>();}
		const Type z() const {return Get<2>();}
	};

} // namespace trm
