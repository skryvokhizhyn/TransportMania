#pragma warning(disable: 4503) // decorated name length exceeded, name was truncated

#include "RoadNetworkImpl.h"
#include "RoadNetwork.h"
#include "RailRoadParametersTaker.h"
#include "RailRoadSizer.h"
#include "PointHash.h"

using namespace trm;
using namespace network_impl;

RoadNetwork::RoadNetwork()
	: implPtr_(std::make_shared<RoadNetworkType>())
{
}

bool 
RoadNetwork::Insert(const RailRoadPtr & p)
{
	RailRoadParametersTaker rrpt;
	p->Accept(rrpt);

	const Point3d & pS = rrpt.GetStart();
	const Point3d & pE = rrpt.GetEnd();

	const size_t pStartHash = trm::hash_value(pS);
	const size_t pEndHash = trm::hash_value(pE);

	RailRoadSizer rrs;
	p->Accept(rrs);

	if (!roads_.Insert(RoadsType::KeyPair(pStartHash, pEndHash), p))
	{
		return false;
	}

	if (!implPtr_->InsertEdge(pStartHash, pEndHash, rrs.GetLenght()))
	{
		if (!roads_.Erase(RoadsType::KeyPair(pStartHash, pEndHash)))
		{
			throw std::runtime_error("De-synchronized roads cache with roads graph");
		}
	}

	return true;
}

RoadRoutePtr 
RoadNetwork::GetRoute(const Point3d & from, const Point3d & to) const
{
	const size_t pStartHash = trm::hash_value(from);
	const size_t pEndHash = trm::hash_value(to);

	const auto route = implPtr_->GetRoute(pStartHash, pEndHash);

	auto r = route.Begin();
	size_t iF = r.Value();
	
	RoadRoute::RoadChunks rc;

	if (route.Size() >= 2)
	{
		while (r.Next())
		{
			size_t iT = r.Value();

			RailRoadPtr p;
			bool direct = true;

			const auto key = RoadsType::KeyPair(iF, iT);

			if (roads_.Find(key, p, direct))
			{
				RailRoadSizer rrs;
				p->Accept(rrs);

				const Heading h = (direct) ? Heading::Forward : Heading::Backward;

				rc.push_back(RoadRoute::RoadChunkType(p, h, rrs.GetLenght()));
			}

			iF = iT;
		}
	}

	return std::make_shared<RoadRoute>(std::move(rc));
}