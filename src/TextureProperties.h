#pragma once

#include "ImgProperties.h"
#include "Types.h"

namespace trm
{
	struct TextureProperties
	{
		GLuintType id;
		utils::ImgProperties props;

		TextureProperties(GLuintType i, const utils::ImgProperties & p)
			: id(i)
			, props(p)
		{}

	};

} // namespace trm
