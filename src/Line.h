#ifndef _LINE_H_
#define _LINE_H_

#include "Types.h"

namespace trm
{
	// ax + by + c = 0
	struct Line
	{
		AxisType a, b, c;

		Line(AxisType i, AxisType k, AxisType l);

		// no test coverage
		AxisType AtX(const AxisType x) const;
		AxisType AtY(const AxisType y) const;
	};

} // namespace trm

#endif // _LINE_H_
