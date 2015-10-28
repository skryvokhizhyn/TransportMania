#pragma once

#include "SceneEventHandler.h"
#include "MoveSceneEventHandlerType.h"

#include <boost/ref.hpp>

namespace trm
{
	class WindowManager;

	class SceneContent
		: boost::noncopyable
	{
	public:
		void SetWindowManager(WindowManager & wm);

		void PutPauseGoButton(bool isPaused);
		void PutSubmitDraftRoadButtons();
		void PutMouseModeButton(MoveSceneEventHandlerType type);

	private:
		WindowManager * pWm_ = nullptr;
	};

} // namespace trm
