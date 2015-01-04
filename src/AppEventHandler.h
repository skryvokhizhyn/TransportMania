#pragma once

#include <SDL_events.h>

namespace trm
{
	class Application;

	class AppEventHandler
	{
	public:
		void Process(const SDL_Event & e, Application & app);

	private:
		void OnKeyDown(const SDL_Event & e, Application & app);
	};

} // namespace trm
