#pragma once

#include "TextureProperties.h"

#include <string>

namespace trm
{
	struct TextureLoader
	{
		static TextureProperties Load(const std::string & path);
		static void Delete(GLuintType id);
	};

} // namespace trm
