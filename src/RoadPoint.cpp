#include "RoadPoint.h"
#include "RoadRoute.h"
#include "RailRoadMover.h"
#include "RailRoadParametersTaker.h"

using namespace trm;

namespace
{
	float LengthLeft(const Heading h, const float length, const float passed)
	{
		if (h == Heading::Forward)
		{
			return length - passed;
		}
		else
		{
			return passed;
		}
	}
}

RoadPoint::RoadPoint(const RoadRoute & r, const Heading h)
	: heading_(h)
	, chunkLengthPassed_(0)
{
	firstIt_ = r.roads_.begin();
	lastIt_ = --r.roads_.end();

	if (heading_ == Heading::Forward)
	{
		currIt_ = firstIt_;
	}
	else
	{
		currIt_ = lastIt_;
	}

	Init(heading_);
}

const Point3d &
RoadPoint::Get() const
{
	return position_;
}

bool
RoadPoint::Shift(const Heading h)
{
	if (h == Heading::Forward)
	{
		if (currIt_ == lastIt_)
		{
			return false;
		}

		++currIt_;
	}
	else
	{
		if (currIt_ == firstIt_)
		{
			return false;
		}

		--currIt_;
	}

	return true;
}

void
RoadPoint::Init(const Heading h)
{
	RailRoadParametersTaker rrpt;
	currIt_->roadPtr->Accept(rrpt);
	
	if (h == Heading::Forward)
	{
		chunkLengthPassed_ = 0;

		if (currIt_->heading == Heading::Forward)
		{
			position_ = rrpt.GetStart();
		}
		else
		{
			position_ = rrpt.GetEnd();
		}
	}
	else
	{
		chunkLengthPassed_ = currIt_->length;

		if (currIt_->heading == Heading::Forward)
		{
			position_ = rrpt.GetEnd();
		}
		else
		{
			position_ = rrpt.GetStart();
		}
	}
}

bool 
RoadPoint::Move(float d)
{ 
	const Heading h = (d > 0) ? heading_ : ((heading_ == Heading::Forward) ? Heading::Backward : Heading::Forward);
	float dist = std::abs(d);
	float lenLeft = 0.0f;
	bool result = true;

	while ((lenLeft = LengthLeft(h, currIt_->length, chunkLengthPassed_)) < dist)
	{
		dist -= lenLeft;

		if (!Shift(h))
		{
			if (!utils::CheckEqual(dist, 0.0f))
			{
				result = false;
			}

			dist = lenLeft;

			break;
		}

		Init(h);
	}
	 
	chunkLengthPassed_ += (h == Heading::Forward) ? dist : -dist;

	RailRoadMover mover(position_, (h == currIt_->heading) ? dist : -dist);
	currIt_->roadPtr->Accept(mover);
	position_ = mover.GetPosition();

	return result;
}