#pragma once

#include <string>

namespace trm
{
	struct FontData;

	struct FontReader
	{
		static FontData Read(const std::string & path);
	};

} // namespace trm
