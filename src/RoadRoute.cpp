#include "RoadRoute.h"
#include "RoadPoint.h"
#include <boost/range/numeric.hpp>

using namespace trm;

RoadRoute::RoadRoute(RoadChunks && roads)
	: roads_(std::move(roads))
{
}

RoadRoute::RoadChunkType::RoadChunkType()
	: length(0.0f)
{}
			
RoadRoute::RoadChunkType::RoadChunkType(RailRoadPtr rp, Heading h, float l)
	: roadPtr(std::move(rp)), heading(h), length(l)
{}

bool
RoadRoute::Valid() const
{
	return !roads_.empty();
}

RoadPoint 
RoadRoute::GetStartingPoint(const Heading h) const
{
	if (!Valid())
	{
		throw std::runtime_error("Emtpy route created");
	}

	return RoadPoint(*this, h);
}

float
RoadRoute::Length() const
{
	return boost::accumulate(roads_, 0.0f,
		[](float i, const RoadChunks::value_type & v)
	{
		return i + v.length;
	});
}