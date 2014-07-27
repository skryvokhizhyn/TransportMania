#pragma once

#include "SDL_events.h"

#include <boost/noncopyable.hpp>

#include <memory>

namespace trm
{
namespace impl
{
	template<typename Subject> class EventStateMachine;

} // namespace impl

	class Application;

	class EventHandler
		: boost::noncopyable
	{
	public:
		EventHandler(Application & app);

		void Process(const SDL_Event & e);
		bool ShouldCommit() const;
		void Commit();

	private:
		// Event parsing methods
		void OnKeyDown(const SDL_Event & e);
		void OnMouseButtonDown(const SDL_Event & e);
		void OnMouseButtonUp(const SDL_Event & e);
		void OnMouseMove(const SDL_Event & e);
		void OnFingerMove(const SDL_Event & e);

	private:
		std::shared_ptr<impl::EventStateMachine<Application>> eventSMPtr_;
		int width_;
		int height_;
	};

} // namespace trm
