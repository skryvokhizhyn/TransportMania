#pragma warning(disable: 4503) // decorated name length exceeded, name was truncated
#pragma warning(push)
#pragma warning(disable: 4127)

#include "TransportManager.h"
#include "TrainStateMachine.h"
#include "RoadPoint.h"
#include "DynamicSceneObjectFactory.h"
#include <boost/range/algorithm.hpp>

using namespace trm;

TransportManager::TransportManager(TrainPtr tPtr, RoadRouteHolder rrH)
	: trainPtr_(std::move(tPtr))
	, passed_(0.0f)
	, distance_(0.0f)
	, speed_(0.0f)
	, rrH_(std::move(rrH))
	, implPtr_(std::make_shared<impl::TrainStateMachine<TransportManager>>(this))
{
}

bool 
TransportManager::Update()
{
	implPtr_->Update();
	
	boost::range::for_each(dsoPtrs_,
		[](const DynamicSceneObjectPtr & dsoPtr)
	{
		dsoPtr->Update();
	});

	return true;
}

bool 
TransportManager::Init()
{
	passed_ = 0.0f;
	rrH_.Next();
	distance_ = rrH_.GetDefinition().roadRotePtr->Length();

	return true;
}

bool 
TransportManager::Load()
{
	trainPtr_->Append(TrainPart(TrainPartType::Wagon));
	trainPtr_->Append(TrainPart(TrainPartType::Wagon));
	trainPtr_->Append(TrainPart(TrainPartType::Wagon));
	trainPtr_->Append(TrainPart(TrainPartType::Locomotive));

	dsoPtrs_ = DynamicSceneObjectFactory::ForTrain(trainPtr_, rrH_.GetDefinition().roadRotePtr->GetStartingPoint(rrH_.GetDefinition().heading));
	Move();

	return true;
}

bool 
TransportManager::Move()
{
	auto && train = *trainPtr_.get();

	passed_ += train.GetMoveDistance();

	if (passed_ >= distance_)
	{
		train.SetMoveDistance(0.0f);

		return true;
	}
	else
	{
		const auto & prm = train.MoveParameters();

		if (distance_ - passed_ < speed_ * speed_ / 2.0f / prm.breaking)
		{
			const float newSpeed = speed_ - prm.breaking;
			if (newSpeed > 0.0f)
			{
				speed_ = newSpeed;
			}
		}
		else
		{
			const float newSpeed = speed_ + prm.acceleration;
			if (newSpeed <= prm.maxSpeed)
			{
				speed_ = newSpeed;
			}
		}

		train.SetMoveDistance(speed_);
	}

	return false;
}
		
bool 
TransportManager::Unload()
{
	trainPtr_->Clear();
	dsoPtrs_.clear();

	return true;
}

const DynamicSceneObjectPtrs & 
TransportManager::GetDynamicSceneObjects() const
{
	return dsoPtrs_;
}

#pragma warning(pop)