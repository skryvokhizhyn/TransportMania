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

#ifdef MOUSE_MOTION_ENABLED

	case SDL_MOUSEBUTTONDOWN:
		OnMouseButtonDown(e);
		break;

	case SDL_MOUSEBUTTONUP:
		OnMouseButtonUp(e);
		break;

	case SDL_MOUSEMOTION:
		OnMouseMove(e);
		break;

#endif // MOUSE_MOTION_ENABLED

	case SDL_FINGERDOWN:
		OnFingerDown(e);
		break;

	case SDL_FINGERUP:
		OnFingerUp(e);
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
	case SDLK_AC_BACK:
		eventSMPtr_->Emit(QuitFired());
		break;
	}
}

void 
EventHandler::OnMouseButtonDown(const SDL_Event & e)
{
	//utils::Logger().Debug() << "Button pressed ";

	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		eventSMPtr_->Emit(FingerPressed(1, Point2d(
			static_cast<float>(e.motion.x), 
			static_cast<float>(e.motion.y))));
		break;
	}
}

void 
EventHandler::OnMouseButtonUp(const SDL_Event & e)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		eventSMPtr_->Emit(FingerReleased{1});
		break;
	}
}

void
EventHandler::OnMouseMove(const SDL_Event & e)
{
	eventSMPtr_->Emit(FingerMoved(1, Point2d(
		static_cast<float>(e.motion.x - e.motion.xrel), 
		static_cast<float>(e.motion.y - e.motion.yrel))));
}

namespace
{
	Point2d GetAbsolutePosition(const SDL_TouchFingerEvent & tfe, int h, int w)
	{
		return Point2d(tfe.x * w, tfe.y * h);
	}
}

void
EventHandler::OnFingerDown(const SDL_Event & e)
{
	//utils::Logger().Debug() << "Finger pressed ";

	const SDL_TouchFingerEvent & tfe = e.tfinger;

	const auto p = GetAbsolutePosition(tfe, width_, height_);

	eventSMPtr_->Emit(FingerPressed{tfe.fingerId, p});
}

void
EventHandler::OnFingerMove(const SDL_Event & e)
{
	const SDL_TouchFingerEvent & tfe = e.tfinger;

	const auto p = GetAbsolutePosition(tfe, width_, height_);

	eventSMPtr_->Emit(FingerMoved{tfe.fingerId, p});
}

void
EventHandler::OnFingerUp(const SDL_Event & e)
{
	const SDL_TouchFingerEvent & tfe = e.tfinger;

	//utils::Logger().Debug() << "Finger UP " << tfe.fingerId;

	eventSMPtr_->Emit(FingerReleased{tfe.fingerId});
}
