#include "MoveSceneEventHandler.h"
#include "EventStateMachine.h"
#include "SceneEventHandlerWrapper.h"
#include "RoadEventStateMachine.h"
#include "Application.h"

using namespace trm;

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

void 
MoveSceneEventHandler::SetMoveHandler(Application & app)
{
	if (handlerPtr_)
	{
		handlerPtr_->Reset();
	}

	handlerPtr_ = MakeSceneWrapper(new impl::EventStateMachine<Application>(app));
}

void 
MoveSceneEventHandler::SetRoadHandler(Application & app)
{
	if (handlerPtr_)
	{
		handlerPtr_->Reset();
	}

	handlerPtr_ = MakeSceneWrapper(new impl::RoadEventStateMachine<Application>(app));
}
