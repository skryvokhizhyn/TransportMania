#include "Train.h"
#include "TrainPartParameters.h"

using namespace trm;

Train::Train(TrainPart head/*, RoadRoutePtr rrp, const Heading h*/)
	/*: roadRoutePtr_(std::move(rrp))
	, heading_(h)*/
	: moveDistance_(0.0f)
	, head_(std::move(head))
	, length_(0.0f)
{
	if (head.type != TrainPartType::Locomotive)
	{
		throw std::runtime_error("Train cannot be moved by wagon");
	}

	ApplyParameters(head_);
}

void
Train::ApplyParameters(const TrainPart & tp)
{
	const auto & data = TrainPartParameters::Get(tp.type);

	moveParams_.maxSpeed = (moveParams_.maxSpeed <= 0.0f) 
		? data.maxSpeed
		: std::min(moveParams_.maxSpeed, data.maxSpeed);

	moveParams_.acceleration = std::max(moveParams_.acceleration, data.acceleration);
	moveParams_.breaking = moveParams_.acceleration * 2.3f;
	
	length_ += data.length;
}

void 
Train::Append(TrainPart tp)
{
	ApplyParameters(tp);

	trainParts_.push_back(std::move(tp));
}

void
Train::Clear()
{
	trainParts_.clear();
}

const TrainParts & 
Train::Parts() const
{
	return trainParts_;
}

//const RoadRoute & 
//Train::Route() const
//{
//	return *(roadRoutePtr_.get());
//}
//
//Heading
//Train::RouteHeading() const
//{
//	return heading_;
//}

void
Train::SetMoveDistance(const float dist)
{
	moveDistance_ = dist;
}

float
Train::GetMoveDistance() const
{
	return moveDistance_;
}

const TrainMoveParameters & 
Train::MoveParameters() const
{
	return moveParams_;
}

float
Train::Length() const
{
	return length_;
}