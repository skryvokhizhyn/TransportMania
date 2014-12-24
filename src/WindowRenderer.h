#pragma once

#include "TextureId.h"

namespace trm
{
	struct ModelData;

	struct WindowRenderer
	{
		static ModelData RenderSquareWindow(const float radii, const TextureId id);
	};

} // namespace trm
