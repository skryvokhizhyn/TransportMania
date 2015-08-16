#pragma once

#include "Point3d.h"
#include "Point3i.h"
#include "RailRoadParametersTaker.h"

#include <utility>

namespace trm
{
namespace road_utils
{
	inline Point3i RoundPoint(const Point3d & p, float precision = 100.0f /*the higher the better*/)
	{
		return Point3i::Cast(p * precision);
	}

	inline std::pair<Point3i, Point3i> GetStartEnd(const RailRoadPtr & p)
	{
		RailRoadParametersTaker rrpt;
		p->Accept(rrpt);

		const Point3d & pStart = rrpt.GetStart();
		const Point3d & pEnd = rrpt.GetEnd();

		return std::make_pair(RoundPoint(pStart), RoundPoint(pEnd));
	}

} // namespace road_utils
} // namespace trm
