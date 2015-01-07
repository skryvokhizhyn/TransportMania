#pragma once

#include "Matrix.h"
#include "DrawableItem.h"
#include "ItemLocator.h"
//#include "EventHandlerWrapper.h"

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

		void CreateOKWindow(EventAction cb);

		void CloseWindow(int id);

		void Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const;

	private:
		using Windows = std::unordered_map<int, DrawableItem>;

	private:
		Windows windows_;
		Size2d screenSize_;
	};

} // namespace trm
