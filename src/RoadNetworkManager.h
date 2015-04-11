#pragma once

#include "RoadNetwork.h"
#include "SymmetricMap.h"
#include "RailRoad.h"
#include "Point3i.h"
#include "RoadRoute.h"

namespace trm
{
	class RoadNetworkManager
	{
	public:
		bool InsertPermanent(const RailRoadPtr & p);
		RoadRoutePtr GetRoute(const Point3d & from, const Point3d & to) const;

		bool InsertTemporary(const RailRoadPtr & p);
		RailRoadPtrs GetTemporary() const;
		void ClearTemporary();

	private:
		using RoadsHodler = SymmetricMap<Point3i, RailRoadPtr>;

	private:
		RoadNetwork roadNetwork_;
		RoadsHodler permRoads_;
		RoadsHodler tempRoads_;
	};

} // namespace trm
