#include "RailRoadIntersection.h"
#include "RailRoadUtils.h"

using namespace trm;

void 
RailRoadIntersectionMap::Insert(const RailRoadIntersection & rri)
{
	const auto startEnd = road_utils::GetStartEnd(rri.roadPtr);
	IntersectionKey key(startEnd.first, startEnd.second);
	const auto found = intersectionsMap_.find(key);

	if (found == intersectionsMap_.end())
	{
		Polygon3d p3d = {rri.intersectionPoint};
		intersectionList_.emplace_back(rri.roadPtr, std::move(p3d));
		intersectionsMap_.emplace(key, --intersectionList_.end());
	}
	else
	{
		found->second->second.push_back(rri.intersectionPoint);
	}
}

auto 
RailRoadIntersectionMap::GetIntersections() const -> const IntersectionList &
{
	return intersectionList_;
}

void 
RailRoadIntersectionMap::Clear()
{
	intersectionsMap_.clear();
	intersectionList_.clear();
}
