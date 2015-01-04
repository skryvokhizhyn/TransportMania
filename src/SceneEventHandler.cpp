#include "SceneEventHandler.h"
#include "EventStateMachine.h"
#include "Application.h"
#include "Logger.h"

//#include <SDL_touch.h>

using namespace trm;
//
//namespace trm
//{
//	// exported for each platform
//	void GetScreenParameters(int & width, int & height, uint32_t & flags);
//}

SceneEventHandler::SceneEventHandler(Application & app)
	: eventSMPtr_(std::make_shared<impl::EventStateMachine<Application>>(app))
	/*, width_(0)
	, height_(0)*/
{
	/*uint32_t flags = 0;

	GetScreenParameters(width_, height_, flags);*/
}

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

//void
//SceneEventHandler::Process(const SDL_Event & e)
//{
//	//utils::Logger().Debug() << "Event type" << e.type;
//
//	switch (e.type)
//	{
//	case SDL_USEREVENT:
//		OnUserEvent(e);
//		break;
//
//#ifdef MOUSE_MOTION_ENABLED
//
//	case SDL_MOUSEBUTTONDOWN:
//		OnMouseButtonDown(e);
//		break;
//
//	case SDL_MOUSEBUTTONUP:
//		OnMouseButtonUp(e);
//		break;
//
//	case SDL_MOUSEMOTION:
//		OnMouseMove(e);
//		break;
//
//#endif // MOUSE_MOTION_ENABLED
//
//	case SDL_FINGERDOWN:
//		OnFingerDown(e);
//		break;
//
//	case SDL_FINGERUP:
//		OnFingerUp(e);
//		break;
//
//	case SDL_FINGERMOTION:
//		OnFingerMove(e);
//		break;
//	}
//}
//
//void 
//SceneEventHandler::OnUserEvent(const SDL_Event & e)
//{
//	switch (e.user.code)
//	{
//	case SdlUserEventType::Commit:
//		eventSMPtr_->Emit(impl::Commit());
//		break;
//	}
//}
//
//void 
//SceneEventHandler::OnMouseButtonDown(const SDL_Event & e)
//{
//	switch (e.button.button)
//	{
//	case SDL_BUTTON_LEFT:
//		eventSMPtr_->Emit(FingerPressed(1, Point2d(
//			static_cast<float>(e.motion.x), 
//			static_cast<float>(e.motion.y))));
//		break;
//	}
//}
//
//void 
//SceneEventHandler::OnMouseButtonUp(const SDL_Event & e)
//{
//	switch (e.button.button)
//	{
//	case SDL_BUTTON_LEFT:
//		eventSMPtr_->Emit(FingerReleased{1});
//		break;
//	}
//}
//
//void
//SceneEventHandler::OnMouseMove(const SDL_Event & e)
//{
//	eventSMPtr_->Emit(FingerMoved(1, Point2d(
//		static_cast<float>(e.motion.x - e.motion.xrel), 
//		static_cast<float>(e.motion.y - e.motion.yrel))));
//}
//
//namespace
//{
//	Point2d GetAbsolutePosition(const SDL_TouchFingerEvent & tfe, int h, int w)
//	{
//		return Point2d(tfe.x * w, tfe.y * h);
//	}
//}
//
//void
//SceneEventHandler::OnFingerDown(const SDL_Event & e)
//{
//	const SDL_TouchFingerEvent & tfe = e.tfinger;
//
//	const auto p = GetAbsolutePosition(tfe, width_, height_);
//
//	eventSMPtr_->Emit(FingerPressed{tfe.fingerId, p});
//}
//
//void
//SceneEventHandler::OnFingerMove(const SDL_Event & e)
//{
//	const SDL_TouchFingerEvent & tfe = e.tfinger;
//
//	const auto p = GetAbsolutePosition(tfe, width_, height_);
//
//	eventSMPtr_->Emit(FingerMoved{tfe.fingerId, p});
//}
//
//void
//SceneEventHandler::OnFingerUp(const SDL_Event & e)
//{
//	const SDL_TouchFingerEvent & tfe = e.tfinger;
//
//	eventSMPtr_->Emit(FingerReleased{tfe.fingerId});
//}
