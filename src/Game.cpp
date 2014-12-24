#define GL_GLEXT_PROTOTYPES

#include "Game.h"
#include "Logger.h"
#include "EventHandler.h"
#include "FpsCounter.h"
#include "UpdateRate.h"
#include "TextManagerProxy.h"

#include <boost/format.hpp>
#include <stdexcept>

using namespace trm;

namespace trm
{
	// exported for each platform
	void GetScreenParameters(int & width, int & height, uint32_t & flags);
}

Game::SDL_RAII::SDL_RAII()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error((boost::format("SDL could not initalize! SDL Error %s") % SDL_GetError()).str());
	}
}

Game::SDL_RAII::~SDL_RAII()
{
	SDL_Quit();
}

Game::Game()
{
	InitSDL();
	InitGL();
	InitApp();
}

Game::~Game()
{
	app_.ReleaseView();
	app_.QuitApplication();
}

void
Game::InitSDL()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	int width = 0;
	int height = 0;
	uint32_t flags = 0;

	GetScreenParameters(width, height, flags);

	windowPtr_.reset(
		SDL_CreateWindow("Transport Mania", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height, flags),
		[](SDL_Window * p) { SDL_DestroyWindow(p); }
	);

	if (!windowPtr_)
	{
		throw std::runtime_error((boost::format("Window could not be created! SDL Error: %s") % SDL_GetError()).str());
	}	
}

void
Game::InitGL()
{
	contextPtr_.reset(
		SDL_GL_CreateContext(windowPtr_.get()),
		[](SDL_GLContext p) { SDL_GL_DeleteContext(p); }
	);

	if (!contextPtr_)
	{
		throw std::runtime_error((boost::format("OpenGL context could not be created! SDL Error: %s") % SDL_GetError()).str());
	}

	if (SDL_GL_SetSwapInterval(-1) < 0)
	{
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			utils::Logger().Warning() << (boost::format("Warning: Unable to set VSync! SDL Error: %s") % SDL_GetError()).str();
		}
	}
}

void
Game::InitApp()
{
	int width = 0;
	int height = 0;
	uint32_t flags = 0;

	GetScreenParameters(width, height, flags);

	app_.InitApplication(width, height);
	app_.InitView();
}

void
Game::Run()
{
	SDL_Event event;
	EventHandler handler(app_);
	
	FpsCounter fpsCounter(1); /*report each 1 seconds*/
	UpdateRate updateRate(30); /* 30 updates per sedond*/

	for (;;)
	{
		while (updateRate.NeedMore())
		{
			while (SDL_PollEvent(&event))
			{
				handler.Process(event);
			}
	
			if (app_.IsStopped())
			{
				return;
			}

			// mark update for the current frame as done
			updateRate.Done();

			// if it's the last update for this frame and we have something to commit
			if (!updateRate.NeedMore() && handler.ShouldCommit())
			{
				handler.Commit();
			}

			app_.Update();
		}
	
		app_.Render();
		app_.Draw();

		SDL_GL_SwapWindow(windowPtr_.get());

		if (fpsCounter.Tick())
		{
			const unsigned frames = fpsCounter.GetFrames();

			//utils::Logger().Debug() << "Frames " << frames;
			TextManagerProxy()->PutFrameRate(frames);
		}

		const unsigned cnt = updateRate.Tick();
		if (cnt > 1)
		{
			utils::Logger().Debug() << "Heavily loaded. Updates " << cnt;
		}
	}
}
