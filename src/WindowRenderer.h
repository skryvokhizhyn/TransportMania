#pragma once

#include "TextureId.h"
#include "Size2d.h"

namespace trm
{
	struct ModelData;

	struct WindowRenderer
	{
		static ModelData RenderRectangleWindow(const Size2d & size, const TextureId id);
	};

} // namespace trm
