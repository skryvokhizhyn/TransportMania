#pragma warning(disable: 4503) // decorated name length exceeded, name was truncated

#include "RoadNetworkImpl.h"
#include "RoadNetwork.h"
#include "Point3i.h"

using namespace trm;
using namespace network_impl;

RoadNetwork::RoadNetwork()
	: implPtr_(new RoadNetworkType())
{
}

RoadNetwork::~RoadNetwork()
{
}

bool 
RoadNetwork::Insert(const Point3i & from, const Point3i & to, float dist)
{
	return implPtr_->InsertEdge(from, to, dist);
}

bool 
RoadNetwork::Remove(const Point3i & from, const Point3i & to)
{
	return implPtr_->RemoveEdge(from, to);
}

RoadNetwork::Route 
RoadNetwork::GetRoute(const Point3i & from, const Point3i & to) const
{
	const auto route = implPtr_->GetRoute(from, to);
	
	Route resultRoute;

	if (route.Size() >= 1)
	{
		auto r = route.Begin();

		resultRoute.push_back(r.Value());

		while (r.Next())
		{
			resultRoute.push_back(r.Value());
		}
	}

	return resultRoute;
}
