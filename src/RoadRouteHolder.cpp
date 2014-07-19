#include "RoadRouteHolder.h"
#include "RoadPoint.h"

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

/////////////////////////////////////

RoadRouteHolder1::RoadRouteHolder1(RoadRoutePtr rrp, const Heading h)
	: roadRoutePtr_(std::move(rrp))
	, heading_(h)
{
}

void
RoadRouteHolder1::Next()
{
	if (heading_ == Heading::Forward)
	{
		heading_ = Heading::Backward;
	}
	else
	{
		heading_ = Heading::Forward;
	}
}

RoadPoint 
RoadRouteHolder1::GetStartingPoint()
{
	return roadRoutePtr_->GetStartingPoint(heading_);
}

float 
RoadRouteHolder1::GetRouteLength()
{
	return roadRoutePtr_->Length();
}