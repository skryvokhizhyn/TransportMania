#pragma once

#include "RailRoad.h"
#include "Point3d.h"

#include <vector>

namespace trm
{
	struct RailRoadIntersection
	{
		RailRoadPtr roadPtr;
		Point3d intersectionPoint;

		RailRoadIntersection(const RailRoadPtr & rrp, const Point3d & ip)
			: roadPtr(rrp)
			, intersectionPoint(ip)
		{}
	};

	using RailRoadIntersections = std::vector<RailRoadIntersection>;

} // namespace trm
