#pragma once

#include "TextManager.h"
#include "DrawContext.h"
#include "FontData.h"
#include <vector>

namespace trm
{
	class TextManagerHub
	{
	public:
		TextManagerHub();

		void Init(const size_t width, const size_t height);

		void PutFrameRate(const unsigned rate);

		void Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const;

	private:
		using TextManagers = std::vector<TextManager>;

	private:
		FontData fontData_;
		size_t width_;
		size_t height_;
		TextManagers textManagers_;
	};

} // namespace trm
