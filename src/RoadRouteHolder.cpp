#include "RoadRouteHolder.h"
#include "RoadPoint.h"

using namespace trm;

RoadRouteHolder::RoadRouteHolder(RoadRoutePtr rrp, const Heading h)
	: roadRoutePtr_(std::move(rrp))
	, heading_(h)
{
}

void
RoadRouteHolder::Next()
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
RoadRouteHolder::GetStartingPoint()
{
	return roadRoutePtr_->GetStartingPoint(heading_);
}

float 
RoadRouteHolder::GetRouteLength()
{
	return roadRoutePtr_->Length();
}