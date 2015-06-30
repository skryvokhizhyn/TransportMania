#include "SceneContent.h"
#include "WindowManager.h"
#include "Logger.h"

#include "SubmitDraftRoadsEvent.h"

using namespace trm;

namespace
{
	void SceneInit(WindowManager & wm)
	{
		wm.CreatePauseButton();
		wm.CreateMouseModeButton();
	}

	void SceneDraw(WindowManager & wm)
	{
		wm.CreateOKNOGroup(SubmitDraftRoadsEvent(true), SubmitDraftRoadsEvent(false));
	}
}

void 
SceneContent::SetWindowManager(WindowManager & wm)
{
	pWm_ = &wm;
}

void
SceneContent::Init(Type t)
{
	assert(pWm_);

	switch (t)
	{
	case Type::Init:
		SceneInit(*pWm_);
		break;

	case Type::Draw:
		if (!drawEnabled_)
		{
			SceneDraw(*pWm_);
			drawEnabled_;
		}
		break;

	case Type::Mode:
		break;

	default:
		utils::Logger().Debug() << "Unknown Scene Type given " << static_cast<int>(t);
	}
}

void
SceneContent::Close(Type t)
{
	assert(pWm_);

	switch (t)
	{
	case Type::Draw:
		drawEnabled_ = false;
		break;

	default:
		break;
	}
}
