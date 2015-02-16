#ifndef _LINE_H_
#define _LINE_H_

#include "Types.h"

namespace trm
{
	// ax + by + c = 0
	struct Line
	{
		const AxisType a, b, c;

		Line(const AxisType i, const AxisType k, const AxisType l);

		// no test coverage
		AxisType AtX(const AxisType x) const;
		AxisType AtY(const AxisType y) const;

		private:
			void operator = (const Line &);
	};

} // namespace trm

#endif // _LINE_H_
