#pragma once

#include "RailRoad.h"
#include "Angle.h"

namespace trm
{
	struct Point2d;
	struct Point3d;

	struct RailRoadFactory
	{
		static RailRoadPtr Line(const Point3d & s, const Point3d & e);
		static RailRoadPtr Arc(const Point3d & s, const Angle a, const Point2d & c, const Rotation r);
	};

} // namespace trm
