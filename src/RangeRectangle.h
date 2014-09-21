#pragma once

#include "Line.h"

namespace trm
{
	struct Point2d;

	struct RangeRectangle
	{
		RangeRectangle(const Line & b, const Line & l1, const Line & l2, const Line & l3, const Line & l4)
			: base(b), ab(l1), bc(l2), cd(l3), da(l4)
		{}

		Line base, ab, bc, cd, da;
	};

	// returns clockwise a, b, c, d
	RangeRectangle GetRangeRectangle(const Point2d & start, const Point2d & end, const AxisType width);
	
} // namespace trm
