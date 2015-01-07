#pragma once

#include "Point2d.h"
#include "Size2d.h"

namespace trm
{
	enum class WindowPosition
	{
		p0,
		p25,
		p50,
		p75,
		p100
	};

	Point2d GetWindowPosition(const Size2d & screenSize, const Size2d & WindowSize, 
		WindowPosition horizontalPos, WindowPosition verticalPos);

} // namespace trm
