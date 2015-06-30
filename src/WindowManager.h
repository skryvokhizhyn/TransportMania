#pragma once

#include "Matrix.h"
#include "DrawableItem.h"
#include "ItemLocator.h"
#include "WindowImpl.h"
#include "WindowItem.h"

#include "EventAction.h"

#include <unordered_map>

namespace trm
{
	class DrawContext;
	class Application;

	class WindowManager
	{
	public:
		void Init(const Size2d & screenSize);

		void CreateOKButton(EventAction cb);
		void CreateNOButton(EventAction cb);
		void CreateOKNOGroup(EventAction cbOK, EventAction cbNO);
		void CreatePauseButton();
		void CreateLockScreen();
		void CreateTextWindow(const std::wstring & text);
		void CreateMouseModeButton();

		void CloseWindow(UniqueId id);

		void Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const;

	private:
		void ProcessWindowData(WindowItemPtr ptr);

	private:
		using Windows = std::unordered_map<UniqueId, WindowImpl>;

	private:
		Windows windows_;
		Size2d screenSize_;
	};

} // namespace trm
