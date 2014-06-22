#pragma once

#define GL_GLEXT_PROTOTYPES

#include <SDL.h>
#include <SDL_opengles2.h>

namespace trm
{
	class Game
	{
	public:
		Game();

		void Init();
		void Run();
		void Term();

	private:
		SDL_Window * gWindow;
		SDL_GLContext gContext;
	};

} // namespace trm
