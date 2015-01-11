#pragma once

#include "FontData.h"
#include "Size2f.h"
#include "ModelData.h"

#include <string>
#include <cstdint>

namespace trm
{
	struct ModelData;


	struct TextData
	{
		ModelData modelData;
		Size2f size;

		TextData(ModelData && md, const Size2f & s);
	};

	class TextRenderer
	{
	public:
		void Init();

		TextData Render(const std::wstring & str) const;
		TextData Render(const std::wstring & str, float fontSize, float maxWidth = -1.0f, float maxHeight = -1.0f) const;
	
	private:
		FontData fontData_;
	};

} // namespace trm
