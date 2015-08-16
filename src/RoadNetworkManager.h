#pragma once

#include "RoadNetwork.h"
#include "SymmetricMap.h"
#include "RailRoad.h"
#include "Point3i.h"
#include "RoadRoute.h"
#include "ItemLocator.h"
#include "RailRoadIntersection.h"

#include <unordered_map>

namespace trm
{
	class UniqueId;
	struct RailRoadConnectionResult;

	class RoadNetworkManager
	{
	public:
		bool InsertPermanentRoad(const RailRoadPtr & p);
		bool RemovePermanentRoad(const RailRoadPtr & p);
		void CommitIntersections();
		RoadRoutePtr GetRoute(const Point3d & from, const Point3d & to) const;

		bool InsertTemporaryRoad(const RailRoadPtr & p);
		void InsertTemporaryIntersections(const RailRoadIntersections & intersections);
		RailRoadPtrs GetTemporaryRoads() const;
		void ClearTemporaryData();

		RailRoadConnectionResult CreateRoad(const Point3d & from, const Point3d & to) const;

	private:
		using RoadMap = std::unordered_map<UniqueId, RailRoadPtr>;
		using RoadSearcher = SymmetricMap<Point3i, UniqueId>;
		using AdjustedPoint = std::pair<Point3d, RailRoadPtr>;

	private:
		AdjustedPoint AdjustPoint(const Point3d & p) const;

	private:
		RoadNetwork roadNetwork_;
		RoadSearcher permRoads_;
		RoadSearcher tempRoads_;
		RailRoadIntersectionMap tempIntersections_;
		RoadMap roadMap_;
		ItemLocator locator_;
	};

} // namespace trm
