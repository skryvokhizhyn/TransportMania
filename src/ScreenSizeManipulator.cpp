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
	assert(screenSize_ != Size2d());

	const float x = (2 * p.x() - screenSize_.x()) / screenSize_.x();
	const float y = (2 * p.y() - screenSize_.y()) / screenSize_.y();

	return Point2d(x, y);
}

Point2d 
ScreenSizeManipulator::ShiftFromCenter(const Point2d & p) const
{
	assert(screenSize_ != Size2d());

	const Point2d p2d = Point2d::Cast(screenSize_ / 2);
	return p - p2d;
}

Polygon2d 
ScreenSizeManipulator::GetScreenPolygon() const
{
	assert(screenSize_ != Size2d());

	const float w = boost::numeric_cast<float>(screenSize_.x());
	const float h = boost::numeric_cast<float>(screenSize_.y());

	return {Point2d(0.0f, 0.0f), Point2d(0.0f, h), Point2d(w, h), Point2d(w, 0.0f)};
}