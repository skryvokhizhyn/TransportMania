#pragma once

#include <string>
#include <cstdint>

namespace trm
{
	struct ModelData;
	struct FontData;

	struct TextRenderer
	{
		static ModelData Render(const std::wstring & str);
		static ModelData Render(const FontData & fd, const std::wstring & str, const std::uint16_t maxWidth);
	};

} // namespace trm
