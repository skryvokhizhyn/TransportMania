#pragma once

#include "SceneEventHandler.h"

#include <boost/ref.hpp>

namespace trm
{
	class WindowManager;

	class SceneContent
		: boost::noncopyable
	{
	public:
		enum class Type
		{
			Init,
			Mode,
			Draw
		};

	public:
		void SetWindowManager(WindowManager & wm);

		void Init(Type t);
		void Close(Type t);

	private:
		WindowManager * pWm_ = nullptr;
		bool drawEnabled_ = false;
	};

} // namespace trm
