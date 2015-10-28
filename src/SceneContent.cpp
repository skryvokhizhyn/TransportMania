#include "SceneContent.h"
#include "WindowManager.h"
#include "Logger.h"

#include "SubmitDraftRoadsEvent.h"

using namespace trm;

void 
SceneContent::SetWindowManager(WindowManager & wm)
{
	pWm_ = &wm;
}

void 
SceneContent::PutMouseModeButton(MoveSceneEventHandlerType type)
{
	assert(pWm_);

	switch (type)
	{
	case MoveSceneEventHandlerType::Move:
		pWm_->CreateSceneMoveButton();
		break;
	
	case MoveSceneEventHandlerType::Draw:
		pWm_->CreateRoadDrawButton();
		break;

	default:
		throw std::runtime_error("Wront mouse mode type given");
	}
}

void
SceneContent::PutPauseGoButton(bool isPaused)
{
	assert(pWm_);

	if (isPaused)
	{
		pWm_->CreateGoButton();
	}
	else
	{
		pWm_->CreatePauseButton();
	}
}

void 
SceneContent::PutSubmitDraftRoadButtons()
{
	assert(pWm_);
	pWm_->CreateOKNOGroup(SubmitDraftRoadsEvent(true), SubmitDraftRoadsEvent(false));
}
