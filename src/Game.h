#pragma once

#include "Application.h"

#include <SDL.h>

#include <memory>
#include <type_traits>

namespace trm
{
	class Game
	{
	public:
		Game();
		~Game();
		void Run();

	private:
		void InitSDL();
		void InitGL();
		void InitApp();

	private:
		using SDLWindowPtr = std::shared_ptr<SDL_Window>;
		using SDLContextPtr = std::shared_ptr<std::remove_pointer<SDL_GLContext>::type>;

	private:
		struct SDL_RAII
		{
			SDL_RAII();
			~SDL_RAII();
		};

	private:
		SDL_RAII sdl_;
		SDLWindowPtr windowPtr_;
		SDLContextPtr contextPtr_;
		Application app_;
	};

} // namespace trm
