#include "EventDispatcher.h"
#include "Application.h"
#include "AppEventHandler.h"
#include "EventHandlerLocatorProxy.h"
#include "SdlUserEvent.h"
#include "Logger.h"

using namespace trm;

namespace trm
{
	// exported for each platform
	void GetScreenParameters(int & width, int & height, uint32_t & flags);
}

EventDispatcher::EventDispatcher(Application & app)
	: app_(app)
{
	uint32_t flags = 0;
	GetScreenParameters(width_, height_, flags);
}

void
EventDispatcher::Process(const SDL_Event & e)
{
	switch (e.type)
	{
	case SDL_QUIT:
	case SDL_KEYDOWN:
	{
		AppEventHandler handler;
		handler.Process(e, app_);
	}
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

template<typename Event>
void 
EventDispatcher::DispatchImpl(const Event & e)
{
	// if handler has changed, reset state of the previous one
	{
		EventHandler * pHandler = EventHandlerLocatorProxy()->At(e.pos);

		if (pHandler != activeHandler_ && activeHandler_ != nullptr)
		{
			activeHandler_->Reset();
		}

		activeHandler_ = pHandler;
	}

	if (activeHandler_)
	{
		activeHandler_->Process(e);
	}
	else
	{
		utils::Logger().Debug() << "No window handler found in pos=" << e.pos << ". Ignoring event";
	}
}

void 
EventDispatcher::OnMouseButtonDown(const SDL_Event & e)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		DispatchImpl(FingerPressed(1, Point2d(
			static_cast<float>(e.motion.x), 
			static_cast<float>(e.motion.y))));
		break;
	}
}

void 
EventDispatcher::OnMouseButtonUp(const SDL_Event & e)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		DispatchImpl(FingerReleased(1, Point2d(
			static_cast<float>(e.motion.x), 
			static_cast<float>(e.motion.y))));
		break;
	}
}

void
EventDispatcher::OnMouseMove(const SDL_Event & e)
{
	DispatchImpl(FingerMoved(1, Point2d(
		static_cast<float>(e.motion.x - e.motion.xrel), 
		static_cast<float>(e.motion.y - e.motion.yrel))));
}

namespace
{
	Point2d GetAbsolutePosition(const SDL_TouchFingerEvent & tfe, int w, int h)
	{
		return Point2d(tfe.x * w, tfe.y * h);
	}
}

void
EventDispatcher::OnFingerDown(const SDL_Event & e)
{
	const SDL_TouchFingerEvent & tfe = e.tfinger;

	const auto p = GetAbsolutePosition(tfe, width_, height_);

	DispatchImpl(FingerPressed{tfe.fingerId, p});
}

void
EventDispatcher::OnFingerMove(const SDL_Event & e)
{
	const SDL_TouchFingerEvent & tfe = e.tfinger;

	const auto p = GetAbsolutePosition(tfe, width_, height_);

	DispatchImpl(FingerMoved{tfe.fingerId, p});
}

void
EventDispatcher::OnFingerUp(const SDL_Event & e)
{
	const SDL_TouchFingerEvent & tfe = e.tfinger;

	const auto p = GetAbsolutePosition(tfe, width_, height_);

	DispatchImpl(FingerReleased{tfe.fingerId, p});
}
