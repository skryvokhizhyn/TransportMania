#include <SDL.h>

namespace trm
{
	void GetScreenParameters(int & width, int & height, uint32_t & flags)
	{
		width = 640;
		height = 480;
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN /*| SDL_WINDOW_FULLSCREEN*/;
	}
}