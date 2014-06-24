//#include "TrainEngine.h"
//#include "TrainPart.h"
//#include "TrainPartParameters.h"
//#include <stdexcept>
//
//using namespace trm;
//
//TrainEngine::TrainEngine(const float dist)
//	: distance_(dist)
//	, passed_(0.0f)
//	, moveStep_(0.0f)
//	, speed_(0.0f)
//	, maxSpeed_(-1.0f)
//	, acceleration_(0.0f)
//	, breaking_(0.0f)
//{
//}
//
//void 
//TrainEngine::SetTrainLength(const float len)
//{
//	if (len > distance_)
//	{
//		throw std::runtime_error("Train is longer than distance");
//	}
//
//	passed_ += len;
//}
//
//float
//TrainEngine::GetMoveDistance() const
//{
//	return moveStep_;
//}
//
//bool 
//TrainEngine::UpdateMoveDistance()
//{
//	passed_ += moveStep_;
//
//	if (passed_ >= distance_)
//	{
//		moveStep_ = 0.0f;
//
//		return false;
//	}
//	else
//	{
//		if (distance_ - passed_ < speed_ * speed_ / 2 / breaking_)
//		{
//			const float newSpeed = speed_ - breaking_;
//			if (newSpeed > 0.0f)
//			{
//				speed_ = newSpeed;
//			}
//		}
//		else
//		{
//			const float newSpeed = speed_ + acceleration_;
//			if (newSpeed <= maxSpeed_)
//			{
//				speed_ = newSpeed;
//			}
//		}
//
//		moveStep_ = speed_;
//	}
//
//	return true;
//}
//
//void
//TrainEngine::Append(const TrainPart & tp)
//{
//	const auto & data = TrainPartParameters::Get(tp.type);
//
//	if (maxSpeed_ < 0.0f)
//	{
//		maxSpeed_ = data.maxSpeed;
//	}
//	else
//	{
//		maxSpeed_ = std::min(maxSpeed_, data.maxSpeed);
//	}
//
//	acceleration_ = std::max(acceleration_, data.acceleration);
//	breaking_ = acceleration_ * 2.3f;
//}
