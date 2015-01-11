#pragma once

#include "Point2d.h"

namespace trm
{
	struct Rectangle2f
	{
		Point2d bottomLeft;
		float width;
		float height;

		Rectangle2f() 
			: width(0.0f), height(0.0f) 
		{}
		Rectangle2f(const Point2d & p, float w, float h)
			: bottomLeft(p), width(w), height(h)
		{}
	};

} // namespace trm
