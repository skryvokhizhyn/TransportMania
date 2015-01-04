#pragma once

#include "SceneEventHandler.h"

#include <SDL_events.h>

#include <boost/noncopyable.hpp>

namespace trm
{
	class Application;

	class EventDispatcher
		: boost::noncopyable
	{
	public:
		EventDispatcher(Application & app);

		void Process(const SDL_Event & e);

	private:
		void OnMouseButtonDown(const SDL_Event & e);
		void OnMouseButtonUp(const SDL_Event & e);
		void OnMouseMove(const SDL_Event & e);
		void OnFingerMove(const SDL_Event & e);
		void OnFingerDown(const SDL_Event & e);
		void OnFingerUp(const SDL_Event & e);

	private:
		Application & app_;
		int width_ = 0;
		int height_ = 0;
	};

} // namespace trm
