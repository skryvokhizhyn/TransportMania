#include "MoveSceneEventHandler.h"
#include "EventStateMachine.h"
#include "SceneEventHandlerWrapper.h"
#include "RoadEventStateMachine.h"
#include "Application.h"

using namespace trm;

MoveSceneEventHandler::MoveSceneEventHandler(Application & app)
	: appRef_(app)
{}

void
MoveSceneEventHandler::Commit() const
{
	handlerPtr_->Commit();
}

void
MoveSceneEventHandler::Process(const FingerPressed & e)
{
	handlerPtr_->Process(e);
}

void
MoveSceneEventHandler::Process(const FingerMoved & e)
{
	handlerPtr_->Process(e);
}

void
MoveSceneEventHandler::Process(const FingerReleased & e)
{
	handlerPtr_->Process(e);
}

void
MoveSceneEventHandler::Reset()
{
	handlerPtr_->Reset();
}

//void 
//MoveSceneEventHandler::ChangeHandler(Application & app)

void 
MoveSceneEventHandler::ChangeHandler(MoveSceneEventHandlerType type)
{
	switch (type)
	{
	case MoveSceneEventHandlerType::Move:
		SetMoveHandler();
		break;

	case MoveSceneEventHandlerType::Draw:
		SetRoadHandler();
		break;

	default:
		throw std::runtime_error("Unknown Scene handler type provided")	;
	}
}

void 
MoveSceneEventHandler::SetMoveHandler()
{
	if (handlerPtr_)
	{
		handlerPtr_->Reset();
	}

	handlerPtr_ = MakeSceneWrapper(new impl::EventStateMachine<Application>(appRef_));
	//type_ = HandlerType::Move;
}

void 
MoveSceneEventHandler::SetRoadHandler()
{
	if (handlerPtr_)
	{
		handlerPtr_->Reset();
	}

	handlerPtr_ = MakeSceneWrapper(new impl::RoadEventStateMachine<Application>(appRef_));
	//type_ = HandlerType::Draw;
}
