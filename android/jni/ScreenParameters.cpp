#include <SDL.h>

namespace trm
{
	void GetScreenParameters(int & width, int & height, uint32_t & flags)
	{
		SDL_DisplayMode mode;

		SDL_GetDisplayMode(0, 0, &mode);
		width = mode.w;
		height = mode.h;

		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
	}
}
