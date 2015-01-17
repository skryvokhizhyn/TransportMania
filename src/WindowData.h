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

		WindowData(UniqueId i, const DrawableItem & d, bool h)
			: id(i), drawableItem(d), hasHandler(h)
		{}
	};

} // namespace trm
