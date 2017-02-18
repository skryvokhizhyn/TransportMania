#pragma once

#include "RoadNetwork.h"
#include "SymmetricMap.h"
#include "RailRoad.h"
#include "Point3i.h"
#include "RoadRoute.h"
#include "ItemLocator.h"
#include "RailRoadIntersection.h"
#include "RailRoadAffectedIds.h"

#include <unordered_map>
#include <unordered_set>

namespace trm
{
	class UniqueId;
	struct RailRoadConnectionResult;

	class RoadNetworkManager
	{
	public:
		OptionalUniqueId InsertPermanentRoad(const RailRoadPtr & p);
		RailRoadAffectedIds CommitTemporaryRoads();
		RoadRoutePtr GetRoute(const Point3d & from, const Point3d & to) const;

		OptionalUniqueId InsertTemporaryRoad(const RailRoadPtr & p);
		void InsertTemporaryIntersections(const RailRoadIntersections & intersections);
		void ClearTemporaryData();

		RailRoadConnectionResult CreateRoad(const Point3d & from, const Point3d & to) const;

		OptionalUniqueId RemovePermanentRoad(const RailRoadPtr & p);
		RailRoadPtr GetRoadById(const UniqueId & id) const;

	private:
		using RoadMap = std::unordered_map<UniqueId, RailRoadPtr>;
		using RoadSearcher = SymmetricMap<Point3i, UniqueId>;
		using AdjustedPoint = std::pair<Point3d, RailRoadPtr>;
		using TempRoadIds = std::unordered_set<UniqueId>;

	private:
		RailRoadPtr GetTouchedRoad(const Point3d & p) const;

	private:
		RoadNetwork roadNetwork_;
		TempRoadIds tempRoadIds_;
		RoadSearcher allRoads_;
		RailRoadIntersectionMap tempIntersections_;
		RoadMap roadMap_;
		ItemLocator locator_;
	};

} // namespace trm
