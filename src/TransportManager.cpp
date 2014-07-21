#pragma warning(disable: 4503) // decorated name length exceeded, name was truncated
#pragma warning(push)
#pragma warning(disable: 4127)

#include "TransportManager.h"
#include "TrainStateMachine.h"
#include "RoadPoint.h"
#include <boost/range/algorithm.hpp>

using namespace trm;

#include "ComponentHolder.h"
#include "TrainPartParameters.h"
#include "TrainMovableObject.h"
#include <cassert>
#include <functional>

TransportManager::TransportManager(ComponentHolder * ch, RoadRouteHolder1 rrh)
	: stateMachinePtr_(std::make_shared<StateMachine>())
	, id_(ComponentIdGenerator::Generate())
	, componentHolderPtr_(ch)
	, distance_(0.0f)
	, speed_(0.0f)
	, passed_(0.0f)
	, rrh_(std::move(rrh))
{
	assert(componentHolderPtr_ != nullptr);
	componentHolderPtr_->trains.insert(std::make_pair(id_, Train(TrainPartType::Locomotive)));
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
	Train & train = componentHolderPtr_->trains.at(id_);

	train.Append(TrainPartType::Wagon);
	train.Append(TrainPartType::Wagon);
	train.Append(TrainPartType::Wagon);
	train.Append(TrainPartType::Locomotive);
	train.Append(TrainPartType::Wagon);

	moveParams_ = train.CalcMoveParams();

	RoadPoint rp = rrh_.GetStartingPoint();

	const auto parts = train.Parts();

	float totalLength = 0.0f;

	std::for_each(parts.crbegin(), parts.crend(),
		[&](const TrainPart & tp)
	{
		componentHolderPtr_->movables.insert(std::make_pair(id_, TrainMovableObject(rp, tp.type)));

		const float partLength = TrainPartParameters::Get(tp.type).length;
		rp.Move(partLength);

		totalLength += partLength;
	});

	componentHolderPtr_->movables.insert(std::make_pair(id_, TrainMovableObject(rp, train.Head().type)));

	if (totalLength > distance_)
	{
		assert(false);
		throw std::runtime_error("Too long train for the route");
	}

	passed_ += totalLength;

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

	auto movables = componentHolderPtr_->movables.equal_range(id_);
	std::for_each(movables.first, movables.second, 
		std::bind(&TrainMovableObject::Move, 		
			std::bind(&ComponentHolder::Movables::value_type::second, std::placeholders::_1), speed_));

	return false;
}
		
bool 
TransportManager::Unload()
{
	Train & train = componentHolderPtr_->trains.at(id_);
	train.ClearParts();

	componentHolderPtr_->movables.erase(id_);

	return true;
}

#pragma warning(pop)