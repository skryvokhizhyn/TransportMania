#ifndef _POINTPROXY_H_
#define _POINTPROXY_H_

#include "PointImpl.h"

namespace trm
{
	template<typename T>
	struct PointProxy : PointImpl<T, 2>
	{
		using Type = typename PointImpl<T, 2>::Type;
		using Base = PointImpl<T, 2>;

		PointProxy(){}
		PointProxy(const PointImpl<T, 2> & pImpl) : Base(pImpl) {}
		PointProxy(const Type a, const Type b) : Base(a, b) {}
		Type & x() {return Base::template Get<0>();}
		const Type x() const {return Base::template Get<0>();}
		Type & y() {return Base::template Get<1>();}
		const Type y() const {return Base::template Get<1>();}
	};

} // namespace

#endif // _POINTPROXY_H_
