#pragma warning(disable: 4503) // decorated name length exceeded, name was truncated
#pragma warning(push)
#pragma warning(disable: 4127)

#include "TransportManager.h"
#include "TrainStateMachine.h"
#include "RoadPoint.h"
#include "ComponentHolderProxy.h"
#include "TrainPartParameters.h"
#include "TrainMovableObject.h"

using namespace trm;

TransportManager::TransportManager(RoadRouteHolder rrh)
	: stateMachinePtr_(std::make_shared<StateMachine>())
	, id_(ComponentIdGenerator::Generate())
	, distance_(0.0f)
	, speed_(0.0f)
	, passed_(0.0f)
	, rrh_(std::move(rrh))
{
	head_ = TrainPartType::Locomotive;
}

void 
TransportManager::Update()
{
	stateMachinePtr_->Update(this);
}

bool 
TransportManager::Init()
{
	distance_ = rrh_.GetRouteLength();
	speed_ = 0.0f;
	passed_ = 0.0f;
	moveParams_ = TrainMoveParameters();

	return true;
}

bool 
TransportManager::Load()
{
	Train train;
	train.Append(head_);
	train.Append(TrainPartType::Wagon);
	train.Append(TrainPartType::Wagon);
	train.Append(TrainPartType::Wagon);
	train.Append(TrainPartType::Locomotive);
	train.Append(TrainPartType::Wagon);

	moveParams_ = train.CalcMoveParams();

	ComponentHolderProxy()->PutTrain(id_, std::move(train), rrh_.GetStartingPoint());

	passed_ += moveParams_.length;

	if (passed_ > distance_)
	{
		assert(false);
		throw std::runtime_error("Too long train for the route");
	}

	return true;
}

bool 
TransportManager::Move()
{
	passed_ += speed_;

	if (passed_ >= distance_)
	{
		speed_ = 0.0f;

		return true;
	}
	else
	{
		if (distance_ - passed_ < speed_ * speed_ / 2.0f / moveParams_.breaking)
		{
			const float newSpeed = speed_ - moveParams_.breaking;
			if (newSpeed > 0.0f)
			{
				speed_ = newSpeed;
			}
		}
		else
		{
			const float newSpeed = speed_ + moveParams_.acceleration;
			if (newSpeed <= moveParams_.maxSpeed)
			{
				speed_ = newSpeed;
			}
		}
	}

	ComponentHolderProxy()->Move(id_, speed_);

	return false;
}
		
bool 
TransportManager::Unload()
{
	ComponentHolderProxy()->Remove(id_);

	return true;
}

#pragma warning(pop)