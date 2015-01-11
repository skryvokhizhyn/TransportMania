#pragma once

#include "DrawableItem.h"
#include "UniqueId.h"

namespace trm
{
	struct WindowData
	{
		UniqueId id;
		DrawableItem drawableItem;
		bool hasHandler;

		WindowData() = delete;
	};

} // namespace trm
