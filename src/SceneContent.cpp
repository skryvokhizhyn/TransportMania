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
	//wm.CreateOKWindow(boost::bind(&WindowManager::CreateTextWindow, boost::ref(windowManager_), L"My first text window!"));
	//wm.CreateOKWindow(boost::bind(&WindowManager::CreateLockScreen, boost::ref(windowManager_)));
	//wm.CreateOKButton(boost::bind(&Application::EmulateDynamicScene1, this));
	/*wm.CreateMouseModeButton(boost::bind(&MoveSceneEventHandler::ChangeHandler, hPtr, std::ref(app)));
	wm.CreatePauseButton();*/
}
