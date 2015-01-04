#pragma once

#include "Matrix.h"
#include "DrawableItem.h"
#include "ItemLocator.h"
#include "EventHandlerWrapper.h"

#include <unordered_map>

namespace trm
{
	class DrawContext;
	class Application;

	class WindowManager
	{
	public:
		void Init(const size_t width, const size_t height);

		void CreateOKWindow(OnEventCallback cb);

		void Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const;

	private:
		using Windows = std::unordered_map<int, DrawableItem>;

	private:
		Windows windows_;
		size_t width_ = 0;
		size_t height_ = 0;
	};

} // namespace trm
