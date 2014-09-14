#pragma once

#include <string>

namespace trm
{
	struct ModelData;

	struct TextRenderer
	{
		static ModelData Render(const std::wstring & str);
	};

} // namespace trm
