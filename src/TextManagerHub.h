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
		void Init();

		void PutFrameRate(const unsigned rate);

		void Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const;

	private:
		using TextManagers = std::vector<TextManager>;

	private:
		FontData fontData_;
		TextManagers textManagers_;
	};

} // namespace trm
