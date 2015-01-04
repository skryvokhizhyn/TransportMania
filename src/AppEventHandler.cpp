#include "AppEventHandler.h"
#include "Application.h"

using namespace trm;

void
AppEventHandler::Process(const SDL_Event & e, Application & app)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		OnKeyDown(e, app);
		break;

	case SDL_QUIT:
		app.Stop();
		break;
	}
}

void 
AppEventHandler::OnKeyDown(const SDL_Event & e, Application & app)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_LEFT:
	case SDLK_a:
		app.ShiftScene(-100.0f, 0.0f);
		app.UpdateTerrain();
		break;

	case SDLK_RIGHT:
	case SDLK_d:
		app.ShiftScene(100.0f, 0.0f);
		app.UpdateTerrain();
		break;

	case SDLK_UP:
	case SDLK_w:
		app.ShiftScene(0.0f, 100.0f);
		app.UpdateTerrain();
		break;

	case SDLK_DOWN:
	case SDLK_s:
		app.ShiftScene(0.0f, -100.0f);
		app.UpdateTerrain();
		break;

	case SDLK_1:
		app.EmulateDynamicScene1();
		break;

	case SDLK_SPACE:
	case SDLK_AC_BACK:
		app.Stop();
		break;

	case SDLK_q:
		app.RotateScene(Degrees(5));
		app.UpdateTerrain();
		break;

	case SDLK_z:
		app.RotateScene(Degrees(-5));
		app.UpdateTerrain();
		break;

	case SDLK_e:
		app.BendScene(Degrees(5), Degrees(0));
		app.UpdateTerrain();
		break;

	case SDLK_c:
		app.BendScene(Degrees(-5), Degrees(0));
		app.UpdateTerrain();
		break;

	case SDLK_r:
		app.BendScene(Degrees(0), Degrees(5));
		app.UpdateTerrain();
		break;

	case SDLK_v:
		app.BendScene(Degrees(0), Degrees(-5));
		app.UpdateTerrain();
		break;

	case SDLK_t:
		app.ZoomScene(5.0f);
		app.UpdateTerrain();
		break;

	case SDLK_b:
		app.ZoomScene(-5.0f);
		app.UpdateTerrain();
		break;
	}
}
