#pragma once

#include "SceneEventHandler.h"

namespace trm
{
	class WindowManager;

	class SceneContent
	{
	public:
		enum class Type
		{
			Init,
			Mode,
			Draw
		};

		static void Init(WindowManager & wm, Type t);
	};

} // namespace trm
