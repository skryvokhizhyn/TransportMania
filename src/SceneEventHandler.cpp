#include "SceneEventHandler.h"
#include "EventStateMachine.h"
#include "Application.h"

using namespace trm;

SceneEventHandler::SceneEventHandler(Application & app)
	: eventSMPtr_(new impl::EventStateMachine<Application>(app))
{}

SceneEventHandler::~SceneEventHandler()
{}

void
SceneEventHandler::Commit() const
{
	eventSMPtr_->Emit(impl::Commit());
}

void
SceneEventHandler::Process(const FingerPressed & e)
{
	eventSMPtr_->Emit(e);
}

void
SceneEventHandler::Process(const FingerMoved & e)
{
	eventSMPtr_->Emit(e);
}

void
SceneEventHandler::Process(const FingerReleased & e)
{
	eventSMPtr_->Emit(e);
}

void
SceneEventHandler::Reset()
{
	eventSMPtr_->Emit(impl::Reset());
}