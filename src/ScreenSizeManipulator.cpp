#include "ScreenSizeManipulator.h"

using namespace trm;

void 
ScreenSizeManipulator::SetScreenSize(const Size2d & sz)
{
	screenSize_ = sz;
}

Point2d 
ScreenSizeManipulator::ConvertToOgl(const Point2d & p) const
{
	const float x = (2 * p.x() - screenSize_.x()) / screenSize_.x();
	const float y = (2 * p.y() - screenSize_.y()) / screenSize_.y();

	return Point2d(x, y);
}

Point2d 
ScreenSizeManipulator::ShiftFromCenter(const Point2d & p) const
{
	const Point2d p2d = Point2d::Cast(screenSize_ / 2);
	return p - p2d;
}
