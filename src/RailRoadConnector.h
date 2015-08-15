#pragma once

#include "Point2d.h"
#include "RailRoad.h"
#include "RailRoadTangentData.h"

namespace trm
{
	using TangentPointPair = std::pair<TangentPoint, TangentPoint>;
	
	struct RailRoadConnectionResult;

	struct RailRoadConnector
	{
		static TangentPointPair GetRoadTangents(const Point3d & pLeft, const RailRoadPtr & rrpLeft, const Point3d & pRight, const RailRoadPtr & rrpRight);

		static RailRoadPtrs GetRoads(const TangentPointPair & tangents);
		static RailRoadConnectionResult GetRoads(Point3d pLeft, const RailRoadPtr & rrpLeft, Point3d pRight, const RailRoadPtr & rrpRight);
	};

} // namespace trm
