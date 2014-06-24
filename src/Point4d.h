#ifndef _POINT4D_H_
#define _POINT4D_H_

#include "PointImpl.h"

namespace trm
{
	struct Point4d : PointImpl<float, 4>
	{
		Point4d(){}
		Point4d(const PointImpl & pImpl) : PointImpl(pImpl) {}
		Point4d(const Type a, const Type b, const Type c, const Type d) : PointImpl(a, b, c, d) {}
		Type & x() {return Get<0>();}
		const Type x() const {return Get<0>();}
		Type & y() {return Get<1>();}
		const Type y() const {return Get<1>();}
		Type & z() {return Get<2>();}
		const Type z() const {return Get<2>();}
		Type & w() {return Get<3>();}
		const Type w() const {return Get<3>();}
	};

} // namespace trm

#endif // _POINT4D_H_