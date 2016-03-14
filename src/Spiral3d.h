#pragma once

#include "Point3d.h"
#include "Point2d.h"
#include "Angle.h"

namespace trm
{
	struct Spiral3d
	{
		const Point3d start;
		const Angle angle;
		const Point2d center;
		const float zShift;

		Spiral3d(Point3d s, Angle a, Point2d c, float z)
			: start(s), angle(a), center(c), zShift(z)
		{}
	};

} // nemaspace trm

