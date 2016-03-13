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
	};

} // nemaspace trm

