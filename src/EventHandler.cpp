#include "EventHandler.h"
#include "EventStateMachine.h"
#include "Application.h"

using namespace trm;
using namespace trm::impl;

#include "Logger.h"

EventHandler::EventHandler(Application & app)
	: eventSMPtr_(std::make_shared<impl::EventStateMachine<Application>>(app))
{}

void
EventHandler::Process(const SDL_Event & e)
{
	//utils::Logger().Debug() << "Event type" << e.type;

	switch (e.type)
	{
	case SDL_QUIT:
		eventSMPtr_->Emit(QuitFired());
		break;

	case SDL_KEYDOWN:
		OnKeyDown(e);
		break;

	case SDL_FINGERDOWN:
		eventSMPtr_->Emit(Key1Pressed());
		break;

	case SDL_MOUSEBUTTONDOWN:
		OnMouseButtonDown(e);
		break;

	case SDL_MOUSEBUTTONUP:
		OnMouseButtonUp(e);
		break;

	case SDL_MOUSEMOTION:
		OnMouseMove(e);
		break;
	}
}
		
void 
EventHandler::OnKeyDown(const SDL_Event & e)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_LEFT:
	case SDLK_a:
		eventSMPtr_->Emit(MoveKeyPressed<MoveKeys::Left>());
		break;

	case SDLK_RIGHT:
	case SDLK_d:
		eventSMPtr_->Emit(MoveKeyPressed<MoveKeys::Right>());
		break;

	case SDLK_UP:
	case SDLK_w:
		eventSMPtr_->Emit(MoveKeyPressed<MoveKeys::Up>());
		break;

	case SDLK_DOWN:
	case SDLK_s:
		eventSMPtr_->Emit(MoveKeyPressed<MoveKeys::Down>());
		break;

	case SDLK_1:
		eventSMPtr_->Emit(Key1Pressed());
		break;

	case SDLK_SPACE:
		eventSMPtr_->Emit(QuitFired());
		break;
	}
}

void 
EventHandler::OnMouseButtonDown(const SDL_Event & e)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		eventSMPtr_->Emit(LMBPressed());
		break;
	}
}

void 
EventHandler::OnMouseButtonUp(const SDL_Event & e)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		eventSMPtr_->Emit(LMBReleased());
		break;
	}
}

void
EventHandler::OnMouseMove(const SDL_Event & e)
{
	eventSMPtr_->Emit(
		MouseMoved(
			static_cast<float>(-e.motion.xrel), 
			static_cast<float>(e.motion.yrel)));
}
