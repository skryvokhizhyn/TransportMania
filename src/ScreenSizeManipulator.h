#pragma once

#include "Size2d.h"
#include "Point2d.h"
#include "Matrix.h"
#include "Polygon2d.h"

namespace trm
{
	class ScreenSizeManipulator
	{
	public:
		void SetScreenSize(const Size2d & sz);

		Point2d ConvertToOgl(const Point2d & p) const;
		Point2d ShiftFromCenter(const Point2d & p) const;
		Polygon2d GetScreenPolygon() const;

	private:
		Size2d screenSize_;
	};

} // namespace trm
