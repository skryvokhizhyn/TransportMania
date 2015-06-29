#pragma once

#include "Point2d.h"
#include "RailRoad.h"
#include "RailRoadTangentData.h"

namespace trm
{
	using TangentPointPair = std::pair<TangentPoint, TangentPoint>;

	struct RailRoadConnector
	{
		static TangentPointPair GetRoadTangents(Point3d pLeft, const RailRoadPtr & rrpLeft, Point3d pRight, const RailRoadPtr & rrpRight);

		static RailRoadPtrs GetRoads(const TangentPointPair & tangents);
		static RailRoadPtrs GetRoads(Point3d pLeft, const RailRoadPtr & rrpLeft, Point3d pRight, const RailRoadPtr & rrpRight);
	};

} // namespace trm
