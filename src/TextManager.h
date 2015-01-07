#pragma once

#include "DrawableItem.h"
#include "DrawContext.h"
#include "FontData.h"
#include <vector>

namespace trm
{
	class TextManager
	{
	public:
		void Init(const Size2d & screenSize);

		void PutFrameRate(const unsigned rate);

		void Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const;

	private:
		using TextManagers = std::vector<DrawableItem>;

	private:
		FontData fontData_;
		Size2d screenSize_;
		TextManagers drawableItems_;
	};

} // namespace trm
