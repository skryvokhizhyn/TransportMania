#pragma once

#include "Point2d.h"
#include "Angle.h"

namespace trm
{
	struct Arc2d
	{
		const Point2d start;
		const Angle angle;
		const Point2d center;
	};

} // namespace trm
