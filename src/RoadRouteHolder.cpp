#include "RoadRouteHolder.h"

using namespace trm;

RoadRouteHolder::RoadRouteHolder(RoadRoutePtr rrp, const Heading h)
{
	rrd_.roadRotePtr = std::move(rrp);
	rrd_.heading = h;
}
		
const RoadRouteDefinition & 
RoadRouteHolder::GetDefinition() const
{
	return rrd_;
}

void
RoadRouteHolder::Next()
{
	if (rrd_.heading == Heading::Forward)
	{
		rrd_.heading = Heading::Backward;
	}
	else
	{
		rrd_.heading = Heading::Forward;
	}
}