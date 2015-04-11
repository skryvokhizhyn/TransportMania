#include "RoadNetworkManager.h"
#include "RailRoadParametersTaker.h"
#include "RailRoadSizer.h"

#include <boost/range/algorithm/transform.hpp>
#include <boost/range/adaptor/map.hpp>

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
}

bool 
RoadNetworkManager::InsertPermanent(const RailRoadPtr & p)
{
	Point3i piStart;
	Point3i piEnd;

	std::tie(piStart, piEnd) = GetStartEnd(p);

	if (!permRoads_.Insert(RoadsHodler::KeyPair(piStart, piEnd), p))
	{
		return false;
	}

	RailRoadSizer rrs;
	p->Accept(rrs);

	if (!roadNetwork_.Insert(piStart, piEnd, rrs.GetLenght()))
	{
		if (!permRoads_.Erase(RoadsHodler::KeyPair(piStart, piEnd)))
		{
			throw std::runtime_error("De-synchronized roads cache with roads graph");
		}

		return false;
	}

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
			RailRoadPtr p;
			bool direct = true;

			if (permRoads_.Find(RoadsHodler::KeyPair(*itPrev, *itNext), p, direct))
			{
				RailRoadSizer rrs;
				p->Accept(rrs);

				const Heading h = (direct) ? Heading::Forward : Heading::Backward;

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
	RoadsHodler::KeyPair key = GetStartEnd(p);

	if (permRoads_.Exists(key))
	{
		return false;
	}

	if (tempRoads_.Exists(key))
	{
		return false;
	}

	if (!tempRoads_.Insert(key, p))
	{
		return false;
	}

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
		boost::bind(&RoadsHodler::ValueType::first, _1));

	return result;
}

void
RoadNetworkManager::ClearTemporary()
{
	tempRoads_.Clear();
}