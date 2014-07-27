#include "EventHandler.h"
#include "EventStateMachine.h"
#include "Application.h"
#include "Logger.h"

#include <SDL_touch.h>

using namespace trm;
using namespace trm::impl;

namespace trm
{
	// exported for each platform
	void GetScreenParameters(int & width, int & height, uint32_t & flags);
}

EventHandler::EventHandler(Application & app)
	: eventSMPtr_(std::make_shared<impl::EventStateMachine<Application>>(app))
	, width_(0)
	, height_(0)
{
	uint32_t flags = 0;

	GetScreenParameters(width_, height_, flags);
}

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

	case SDL_MOUSEBUTTONDOWN:
	case SDL_FINGERDOWN:
		OnMouseButtonDown(e);
		break;

	case SDL_MOUSEBUTTONUP:
	case SDL_FINGERUP:
		OnMouseButtonUp(e);
		break;

	case SDL_MOUSEMOTION:
		OnMouseMove(e);
		break;

	case SDL_FINGERMOTION:
		OnFingerMove(e);
		break;
	}
}
	
void
EventHandler::Commit()
{
	eventSMPtr_->Emit(impl::Commit());
}

bool
EventHandler::ShouldCommit() const
{
	return eventSMPtr_->Commitable();
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

void
EventHandler::OnFingerMove(const SDL_Event & e)
{
	const SDL_TouchFingerEvent & tfe = e.tfinger;

	const float dx = (tfe.dx * width_);
	const float dy = (tfe.dy * height_);

	eventSMPtr_->Emit(MouseMoved{-dx, dy});
}