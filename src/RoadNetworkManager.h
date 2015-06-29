#pragma once

#include "RoadNetwork.h"
#include "SymmetricMap.h"
#include "RailRoad.h"
#include "Point3i.h"
#include "RoadRoute.h"
#include "ItemLocator.h"

#include <unordered_map>

namespace trm
{
	class UniqueId;

	class RoadNetworkManager
	{
	public:
		bool InsertPermanent(const RailRoadPtr & p);
		RoadRoutePtr GetRoute(const Point3d & from, const Point3d & to) const;

		bool InsertTemporary(const RailRoadPtr & p);
		RailRoadPtrs GetTemporary() const;
		void ClearTemporary();

		RailRoadPtrs CreateRoad(const Point3d & from, const Point3d & to) const;

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
		RoadMap roadMap_;
		ItemLocator locator_;
	};

} // namespace trm
