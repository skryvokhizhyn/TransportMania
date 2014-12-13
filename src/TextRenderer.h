#pragma once

#include <string>

namespace trm
{
	struct ModelData;
	struct FontData;

	struct TextRenderer
	{
		static ModelData Render(const std::wstring & str);
		static ModelData Render(const FontData & fd, const std::wstring & str);
	};

} // namespace trm
