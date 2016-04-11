#pragma once

#include "Point2d.h"
#include "RailRoad.h"
#include "RailRoadTangentData.h"
#include "PointWithDirection2d.h"

namespace trm
{
	using TangentPointPair = std::pair<TangentPoint, TangentPoint>;
	
	struct RailRoadConnectionResult;

	struct RailRoadConnector
	{
		static TangentPointPair GetRoadTangents(const Point3d & pLeft, const RailRoadPtr & rrpLeft, const Point3d & pRight, const RailRoadPtr & rrpRight);

		static RailRoadPtrs GetRoads(const TangentPointPair & tangents);
		static RailRoadConnectionResult GetRoads(Point3d pLeft, const RailRoadPtr & rrpLeft, Point3d pRight, const RailRoadPtr & rrpRight);

		// returns middle point (can be improved to slight this point if angles aren't equal) if directions look at different sides
		static Point2d GetArcsConnectionPoint(const PointWithDirection2d & pointDirFrom, const PointWithDirection2d & pointDirTo);
	};

} // namespace trm
