#include "Train.h"
#include "TrainPartParameters.h"

#include <boost/range/algorithm.hpp>

using namespace trm;

Train::Train()
{}

Train::Train(Train && t)
	: position_(std::move(t.position_))
	, parts_(std::move(t.parts_))
{
}

void
Train::SetRoadPoint(RoadPoint rp)
{
	position_ = rp;
}

void
Train::Append(TrainPart tp)
{
	parts_.push_back(std::move(tp));
}

void
Train::ClearParts()
{
	parts_.clear();
}

//const TrainPart &
//Train::Head() const
//{
//	return head_;
//}

const TrainParts &
Train::Parts() const
{
	return parts_;
}

TrainMoveParameters
Train::CalcMoveParams()
{
	TrainMoveParameters moveParams;

	boost::for_each(parts_,
		[&](const TrainPart & tp)
	{
		const auto & data = TrainPartParameters::Get(tp.type);

		moveParams.maxSpeed = (moveParams.maxSpeed <= 0.0f) 
			? data.maxSpeed
			: std::min(moveParams.maxSpeed, data.maxSpeed);

		moveParams.acceleration = std::max(moveParams.acceleration, data.acceleration);
		moveParams.breaking = moveParams.acceleration * 1.3f;
		moveParams.length += data.length;
	});

	return moveParams;
}