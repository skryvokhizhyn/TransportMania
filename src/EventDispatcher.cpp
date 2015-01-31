#include "EventDispatcher.h"
#include "Application.h"
#include "AppEventHandler.h"
#include "CachedHandlerLocatorProxy.h"
#include "SdlUserEvent.h"
#include "Logger.h"
#include "SdlUserEventVisitor.h"
#include "SdlUserEvent.h"

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

	case SDL_USEREVENT:
	{
		SdlUserEventVisitor	visitor(app_);
		SdlUserEventCoder::Decode(e)->Accept(visitor);
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

void 
EventDispatcher::OnMouseButtonDown(const SDL_Event & e)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		CachedHandlerLocatorProxy()->Process(FingerPressed(1, Point2d(
			static_cast<float>(e.motion.x), 
			static_cast<float>(height_ - e.motion.y))));
		break;
	}
}

void 
EventDispatcher::OnMouseButtonUp(const SDL_Event & e)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		CachedHandlerLocatorProxy()->Process(FingerReleased(1, Point2d(
			static_cast<float>(e.motion.x), 
			static_cast<float>(height_ - e.motion.y))));
		break;
	}
}

void
EventDispatcher::OnMouseMove(const SDL_Event & e)
{
	CachedHandlerLocatorProxy()->Process(FingerMoved(1, Point2d(
		static_cast<float>(e.motion.x - e.motion.xrel), 
		static_cast<float>(height_ - (e.motion.y - e.motion.yrel)))));
}

namespace
{
	Point2d GetAbsolutePosition(const SDL_TouchFingerEvent & tfe, int w, int h)
	{
		return Point2d(tfe.x * w, h - tfe.y * h);
	}
}

void
EventDispatcher::OnFingerDown(const SDL_Event & e)
{
	const SDL_TouchFingerEvent & tfe = e.tfinger;

	const auto p = GetAbsolutePosition(tfe, width_, height_);

	CachedHandlerLocatorProxy()->Process(FingerPressed{tfe.fingerId, p});
}

void
EventDispatcher::OnFingerMove(const SDL_Event & e)
{
	const SDL_TouchFingerEvent & tfe = e.tfinger;

	const auto p = GetAbsolutePosition(tfe, width_, height_);

	CachedHandlerLocatorProxy()->Process(FingerMoved{tfe.fingerId, p});
}

void
EventDispatcher::OnFingerUp(const SDL_Event & e)
{
	const SDL_TouchFingerEvent & tfe = e.tfinger;

	const auto p = GetAbsolutePosition(tfe, width_, height_);

	CachedHandlerLocatorProxy()->Process(FingerReleased{tfe.fingerId, p});
}
