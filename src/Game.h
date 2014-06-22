#pragma once

#include <SDL.h>

#include <memory>
#include <type_traits>

namespace trm
{
	class Game
	{
	public:
		Game();
		void Run();

	private:
		void InitSDL();
		void InitGL();

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
	};

} // namespace trm
