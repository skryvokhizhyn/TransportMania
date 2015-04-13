#include "RoadNetworkManager.h"
#include "UniqueId.h"
#include "TerrainRangeCircle.h"
#include "Point2d.h"
#include "Point2i.h"

#include "RailRoadRangeGenerator.h"
#include "RailRoadClosestPoint.h"
#include "RailRoadParametersTaker.h"
#include "RailRoadSizer.h"

#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/reversed.hpp>

using namespace trm;

namespace
{
	Point3i RoundPoint(const Point3d & p)
	{
		// the higher the better
		const int precision = 100;

		return Point3i::Cast(p * precision);
	}

	std::pair<Point3i, Point3i> GetStartEnd(const RailRoadPtr & p)
	{
		RailRoadParametersTaker rrpt;
		p->Accept(rrpt);

		const Point3d & pStart = rrpt.GetStart();
		const Point3d & pEnd = rrpt.GetEnd();

		return std::make_pair(RoundPoint(pStart), RoundPoint(pEnd));
	}

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
		RailRoadRangeGenerator rrrg;
		p->Accept(rrrg);

		return ConvertRangeToPolygon(rrrg.GetRange().GetRanges());
	}
}

bool 
RoadNetworkManager::InsertPermanent(const RailRoadPtr & p)
{
	Point3i piStart;
	Point3i piEnd;

	std::tie(piStart, piEnd) = GetStartEnd(p);

	const UniqueId newId = roadMap_.emplace(UniqueId::Generate(), p).first->first;

	if (!permRoads_.Insert(RoadSearcher::KeyPair(piStart, piEnd), newId))
	{
		return false;
	}

	RailRoadSizer rrs;
	p->Accept(rrs);

	if (!roadNetwork_.Insert(piStart, piEnd, rrs.GetLenght()))
	{
		if (!permRoads_.Erase(RoadSearcher::KeyPair(piStart, piEnd)))
		{
			throw std::runtime_error("De-synchronized roads cache with roads graph");
		}

		return false;
	}

	locator_.Put(newId, ConvertRoadToPolygon(p));

	return true;
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
			const auto found = permRoads_.Find(RoadSearcher::KeyPair(*itPrev, *itNext));

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

bool 
RoadNetworkManager::InsertTemporary(const RailRoadPtr & p)
{
	RoadSearcher::KeyPair key = GetStartEnd(p);

	if (permRoads_.Exists(key))
	{
		return false;
	}

	if (tempRoads_.Exists(key))
	{
		return false;
	}

	UniqueId newId = roadMap_.emplace(UniqueId::Generate(), p).first->first;

	if (!tempRoads_.Insert(key, newId))
	{
		return false;
	}

	locator_.Put(newId, ConvertRoadToPolygon(p));

	return true;
}

RailRoadPtrs
RoadNetworkManager::GetTemporary() const
{
	RailRoadPtrs result;

	const auto & data = tempRoads_.Data();

	result.reserve(data.size());

	using namespace boost::adaptors;

	boost::transform(data | map_values, std::back_inserter(result),
		[&](const RoadSearcher::ValueType & val)
	{
		return roadMap_.at(val.first);
	});

	return result;
}

void
RoadNetworkManager::ClearTemporary()
{
	using namespace boost::adaptors;

	boost::for_each(tempRoads_.Data() | map_values, 
		[&](const RoadSearcher::ValueType & val)
	{
		roadMap_.erase(val.first);
	});

	tempRoads_.Clear();
}

Point2d 
RoadNetworkManager::AdjustPoint(const Point2d & p) const
{
	TerrainRangeCircle circle(p, 1.0f);

	auto ids = locator_.At(ConvertRangeToPolygon(circle.GetRanges()));

	if (ids.empty())
		return p;

	const RailRoadPtr & foundRoad = roadMap_.at(ids.front());

	RailRoadClosestPoint rrcp(p, true);
	foundRoad->Accept(rrcp);

	return rrcp.GetPoint();
}
