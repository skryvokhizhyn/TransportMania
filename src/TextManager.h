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
		TextManager();

		void Init(const size_t width, const size_t height);

		void PutFrameRate(const unsigned rate);

		void Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const;

	private:
		using TextManagers = std::vector<DrawableItem>;

	private:
		FontData fontData_;
		size_t width_;
		size_t height_;
		TextManagers drawableItems_;
	};

} // namespace trm
