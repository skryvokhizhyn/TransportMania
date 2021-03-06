#define GL_GLEXT_PROTOTYPES

#include "Game.h"
#include "Logger.h"
#include "EventDispatcher.h"
#include "FpsCounter.h"
#include "UpdateRate.h"
#include "TextManagerProxy.h"
#include "SdlUserEvent.h"
#include "RenderRateGuard.h"

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
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    
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

	int value = 0;
    if (SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &value) == 0) 
	{
		utils::Logger().Debug() << "Multi samples count is " << value;
    }
	else
	{
		utils::Logger().Debug() << "Failed to get multi samples count. Error: " << SDL_GetError();
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
	EventDispatcher dispatcher(app_);
	
	FpsCounter fpsCounter(1); /* report each 1 seconds */
	UpdateRate updateRate(30); /* 30 updates per sedond */
	RenderRateGuard renderRateGuard(30); /* 30 renders per second*/

	for (;;)
	{
		// makes "updateRate" updates per second
		// sacrifices with rendering so updates happen with 
		// expected rate
		while (updateRate.NeedMore())
		{
			while (SDL_PollEvent(&event))
			{
				dispatcher.Process(event);
			}
	
			if (app_.IsStopped())
			{
				return;
			}

			// mark update for the current frame as done
			updateRate.Done();

			// if it's the last update for this frame
			if (!updateRate.NeedMore())
			{
				app_.CommitSceneActions();
			}
		}
	
		app_.Update();
		app_.Render();
		app_.Draw();

		SDL_GL_SwapWindow(windowPtr_.get());

		const unsigned cnt = updateRate.Tick();
		if (cnt > 1)
		{
			utils::Logger().Debug() << "Heavily loaded. Updates " << cnt;
		}

		// guarantees that render rate will be as specified by render rate guard
		auto load = renderRateGuard.Snooze();

		if (fpsCounter.Tick())
		{
			const unsigned frames = fpsCounter.GetFrames();

			TextManagerProxy()->PutFrameRate(frames, load);
		}
	}
}
