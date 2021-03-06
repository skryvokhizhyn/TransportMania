#pragma once

#include "RailRoad.h"
#include "Angle.h"

namespace trm
{
	struct Point2d;
	struct Point3d;

	struct RailRoadFactory
	{
		static RailRoadPtr Line(const Point3d & start, const Point3d & end);
		static RailRoadPtr Arc(const Point3d & start, const Angle angle, const Point2d & center, float zShift = 0.0f);
		static RailRoadPtr Arc(const Point3d & start, const Point2d & direction, const Point3d & end);
	};

} // namespace trm
