#include "TrainMovableObject.h"

using namespace trm;

TrainMovableObject::TrainMovableObject(RoadPoint rp, TrainPartType type)
	: roadPoint_(std::move(rp))
	, type_(type)
	, sharedPosition_()
	, visible_(false)
{}

void 
TrainMovableObject::Move(const float dist)
{
	roadPoint_.Move(dist);

	if (sharedPosition_)
	{
		*sharedPosition_ = Position();
	}
}

const Point3d & 
TrainMovableObject::Position() const
{
	return roadPoint_.Get();
}

Point3d 
TrainMovableObject::MovedPosition(const float dist) const
{
	RoadPoint temp = roadPoint_;
	temp.Move(dist);

	return temp.Get();
}

TrainPartType 
TrainMovableObject::Type() const
{
	return type_;
}

bool 
TrainMovableObject::GetVisible() const
{
	return visible_;
}

void 
TrainMovableObject::SetVisible(bool v)
{
	visible_ = v;

	if (visible_)
	{
		sharedPosition_ = std::make_shared<Point3d>(Position());
	}
	else
	{
		sharedPosition_.reset();
	}
}

PositionWPtr 
TrainMovableObject::SharedPosition() const
{
	return sharedPosition_;
}