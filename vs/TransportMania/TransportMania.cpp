#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <SDL.h>

int main(int /*argc*/, char ** /*argv*/)
{
	SDL_Window * window = nullptr;
	SDL_GLContext gl = 0;

	if (0 != SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "\nunable to initialize SDL: %s\n", SDL_GetError());
	}

	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_DisplayMode mode;

	SDL_GetDisplayMode(0, 0, &mode);
	int width = mode.w;
	int height = mode.h;

	SDL_Log("Width = %d, Height = %d\n", width, height);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	window = SDL_CreateWindow(NULL, 50, 50, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE);

	if (window == 0)
	{
		SDL_Log("Failed to create window");
		SDL_Quit();
		return 1;
	}

	gl = SDL_GL_CreateContext(window);

	Uint8 done = 0;
	SDL_Event event;
	int count = 0;

	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN || event.type == SDL_FINGERDOWN)
			{
				done = 1;
			}
		}

		SDL_Log("%d\n", count++);

		SDL_GL_SwapWindow(window);
		SDL_Delay(10);
	}

	exit(0);
}