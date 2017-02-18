#include "RoadNetworkManager.h"
#include "UniqueId.h"
#include "TerrainRangeCircle.h"
#include "Point2d.h"
#include "Point2i.h"

#include "RailRoadRangeGenerator.h"
#include "RailRoadParametersTaker.h"
#include "RailRoadSizer.h"
#include "RailRoadConnector.h"
#include "RailRoadSplitter.h"

#include "RailRoadUtils.h"

#include "RailRoadConnectionResult.h"

#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/reversed.hpp>

using namespace trm;
using namespace trm::road_utils;

namespace
{
	Polygon2d ConvertRangeToPolygon(const TerrainRange::Ranges & ranges)
	{
		Polygon2d result;
		result.reserve(ranges.size() * 2);

		boost::transform(ranges, std::back_inserter(result), 
			[](const TerrainRange::Range & range)
		{
			return Point2d::Cast(Point2i(range.xBegin, range.y));
		});

		boost::transform(ranges | boost::adaptors::reversed, std::back_inserter(result), 
			[](const TerrainRange::Range & range)
		{
			return Point2d::Cast(Point2i(range.xEnd, range.y));
		});

		return result;
	}

	Polygon2d ConvertRoadToPolygon(const RailRoadPtr & p)
	{
		RailRoadRangeGenerator rrrg(false);
		p->Accept(rrrg);

		return ConvertRangeToPolygon(rrrg.GetRange().GetRanges());
	}
}

OptionalUniqueId 
RoadNetworkManager::InsertPermanentRoad(const RailRoadPtr & p)
{
	const RoadSearcher::KeyPair keyPair = GetStartEnd(p);

	const UniqueId newId = roadMap_.emplace(UniqueId::Generate(), p).first->first;

	if (!allRoads_.Insert(keyPair, newId))
	{
		return OptionalUniqueId();
	}

	RailRoadSizer rrs;
	p->Accept(rrs);

	if (!roadNetwork_.Insert(keyPair.first, keyPair.second, rrs.GetLenght()))
	{
		if (!allRoads_.Erase(keyPair))
		{
			throw std::runtime_error("De-synchronized roads cache with roads graph");
		}

		return OptionalUniqueId();
	}

	locator_.Put(newId, ConvertRoadToPolygon(p));

	return newId;
}

OptionalUniqueId 
RoadNetworkManager::RemovePermanentRoad(const RailRoadPtr & p)
{
	const RoadSearcher::KeyPair keyPair = GetStartEnd(p);

	const auto roadId = allRoads_.Find(keyPair);

	if (!roadId)
	{
		return OptionalUniqueId();
	}

	locator_.Remove(roadId->first);
	roadMap_.erase(roadId->first);

	if (!allRoads_.Erase(keyPair))
	{
		throw std::runtime_error("Trying to remove not existing road");
	}

	if (!roadNetwork_.Remove(keyPair.first, keyPair.second))
	{
		throw std::runtime_error("Failed to remove permanent road from road network");
	}

	return roadId->first;
}

RoadRoutePtr 
RoadNetworkManager::GetRoute(const Point3d & from, const Point3d & to) const
{
	const Point3i piStart = RoundPoint(from);
	const Point3i piEnd = RoundPoint(to);

	const auto route = roadNetwork_.GetRoute(piStart, piEnd);

	RoadRoute::RoadChunks rc;

	if (route.size() >= 2)
	{
		auto itNext = route.begin();
		auto itPrev = itNext++;

		while (itNext != route.end())
		{
			const auto found = allRoads_.Find(RoadSearcher::KeyPair(*itPrev, *itNext));

			if (found)
			{
				const RailRoadPtr & p = roadMap_.at(found->first);

				RailRoadSizer rrs;
				p->Accept(rrs);

				const Heading h = (found->second) ? Heading::Forward : Heading::Backward;

				rc.push_back(RoadRoute::RoadChunkType(p, h, rrs.GetLenght()));
			}

			itPrev = itNext++;
		}
	}

	return std::make_shared<RoadRoute>(std::move(rc));
}

OptionalUniqueId 
RoadNetworkManager::InsertTemporaryRoad(const RailRoadPtr & p)
{
	RoadSearcher::KeyPair key = GetStartEnd(p);

	if (allRoads_.Exists(key))
	{
		return OptionalUniqueId();
	}

	UniqueId newId = roadMap_.emplace(UniqueId::Generate(), p).first->first;

	if (!tempRoadIds_.insert(newId).second)
	{
		return OptionalUniqueId();
	}

	if (!allRoads_.Insert(key, newId))
	{
		return OptionalUniqueId();
	}

	locator_.Put(newId, ConvertRoadToPolygon(p));

	return newId;
}

void 
RoadNetworkManager::InsertTemporaryIntersections(const RailRoadIntersections & intersections)
{
	boost::for_each(intersections, boost::bind(&RailRoadIntersectionMap::Insert, boost::ref(tempIntersections_), _1));
}

void
RoadNetworkManager::ClearTemporaryData()
{
	using namespace boost::adaptors;

	boost::for_each(tempRoadIds_, 
		[&](const UniqueId & id)
	{
		roadMap_.erase(id);
		locator_.Remove(id);
	});

	tempRoadIds_.clear();
}

RailRoadPtr
RoadNetworkManager::GetTouchedRoad(const Point3d & p) const
{
	const Point2d p2d = Point2d::Cast(p);

	TerrainRangeCircle circle(p2d, 0.0f);

	auto ids = locator_.At(ConvertRangeToPolygon(circle.GetRanges()));

	if (ids.empty())
		return RailRoadPtr();

	const RailRoadPtr & foundRoad = roadMap_.at(ids.front());

	return foundRoad;
}

RailRoadConnectionResult 
RoadNetworkManager::CreateRoad(const Point3d & from, const Point3d & to) const
{
	if (from == to)
	{
		return RailRoadConnectionResult();
	}

	const RailRoadPtr & touchedFrom = GetTouchedRoad(from);
	const RailRoadPtr & touchedTo = GetTouchedRoad(to);

	return RailRoadConnector::GetRoads(from, touchedFrom, to, touchedTo);
}

RailRoadAffectedIds 
RoadNetworkManager::CommitTemporaryRoads()
{
	boost::for_each(tempRoadIds_,
		[&](const UniqueId & id)
	{
		const auto & p = roadMap_.at(id);

		RailRoadSizer rrs;
		p->Accept(rrs);

		RoadSearcher::KeyPair key = GetStartEnd(p);

		roadNetwork_.Insert(key.first, key.second, rrs.GetLenght());
	});

	RailRoadAffectedIds result(RailRoadAffectedIds::Permanent);
	result.addedIds.insert(result.addedIds.end(), tempRoadIds_.begin(), tempRoadIds_.end());

	tempRoadIds_.clear();

	boost::for_each(tempIntersections_.GetIntersections(), 
		[&](const RailRoadIntersectionMap::IntersectionValue & val)
	{
		RailRoadSplitter rrs(val.second);
		val.first->Accept(rrs);

		const RailRoadSplitResult & splitResult = rrs.GetSplitResult();

		if (splitResult.empty())
		{
			return;
		}

		const OptionalUniqueId removedId = RemovePermanentRoad(val.first);

		if (removedId)
		{
			result.removedIds.push_back(removedId.get());
			result.addedIds.reserve(result.addedIds.size() + splitResult.size());

			for (const auto & roadToInsert : splitResult)
			{
				const OptionalUniqueId insertedId = InsertPermanentRoad(roadToInsert);

				if (insertedId)
				{
					result.addedIds.push_back(insertedId.get());
				}
			}
		}
	});

	// have to clear it here as second run of commit will create duplicates
	tempIntersections_.Clear();

	return result;
}

RailRoadPtr 
RoadNetworkManager::GetRoadById(const UniqueId & id) const
{
	auto it = roadMap_.find(id);

	if (it == roadMap_.end())
	{
		return RailRoadPtr();
	}

	return it->second;
}
