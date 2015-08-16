#pragma once

#include "RailRoad.h"
#include "Point3d.h"
#include "Point3i.h"
#include "Polygon3d.h"

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

#include <map>
#include <list>

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

	class RailRoadIntersectionMap
	{
	public:
		using IntersectionValue = std::pair<RailRoadPtr, Polygon3d>;
		using IntersectionList = std::list<IntersectionValue>;

	public:
		void Insert(const RailRoadIntersection & rri);
		const IntersectionList & GetIntersections() const;
		void Clear();

	private:
		using IntersectionKey = boost::tuple<Point3i, Point3i>;
		using IntersectionMap = std::map<IntersectionKey, IntersectionList::iterator>;

	private:
		IntersectionList intersectionList_;
		IntersectionMap intersectionsMap_;
	};

} // namespace trm
