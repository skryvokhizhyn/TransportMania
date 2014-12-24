#pragma once

#include "Matrix.h"
#include "DrawableItem.h"

#include <vector>

namespace trm
{
	class DrawContext;

	class WindowManager
	{
	public:
		void Init(const size_t width, const size_t height);

		void CreateOKWindow();

		void Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const;

	private:
		using Windows = std::vector<DrawableItem>;

	private:
		Windows windows_;
		size_t width_ = 0;
		size_t height_ = 0;
	};

} // namespace trm
