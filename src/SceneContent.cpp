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
		wm.CreateOKButton(SubmitDraftRoadsEvent(true));
		wm.CreateNOButton(SubmitDraftRoadsEvent(false));
	}
}

void
SceneContent::Init(WindowManager & wm, Type t)
{
	switch (t)
	{
	case Type::Init:
		SceneInit(wm);
		break;

	case Type::Draw:
		SceneDraw(wm);
		break;

	case Type::Mode:
		break;

	default:
		utils::Logger().Debug() << "Unknown Scene Type given " << static_cast<int>(t);
	}
}
