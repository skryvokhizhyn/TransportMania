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

	private:
		// Event parsing methods
		void OnKeyDown(const SDL_Event & e);

	private:
		std::shared_ptr<impl::EventStateMachine<Application>> eventSMPtr_;
	};

} // namespace trm
