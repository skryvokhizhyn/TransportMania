#include "Train.h"
#include "TrainPartParameters.h"

#include <boost/range/algorithm.hpp>

using namespace trm;

Train::Train(TrainPart head)
	: head_(std::move(head))
{}

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

const TrainPart &
Train::Head() const
{
	return head_;
}

const TrainParts &
Train::Parts() const
{
	return parts_;
}

TrainMoveParameters
Train::CalcMoveParams()
{
	TrainMoveParameters moveParams_;

	boost::for_each(parts_,
		[&](const TrainPart & tp)
	{
		const auto & data = TrainPartParameters::Get(tp.type);

		moveParams_.maxSpeed = (moveParams_.maxSpeed <= 0.0f) 
			? data.maxSpeed
			: std::min(moveParams_.maxSpeed, data.maxSpeed);

		moveParams_.acceleration = std::max(moveParams_.acceleration, data.acceleration);
		moveParams_.breaking = moveParams_.acceleration * 1.3f;
	});

	return moveParams_;
}