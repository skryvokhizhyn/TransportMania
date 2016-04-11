#include "LinearHeightGetter.h"
#include "Line.h"
#include "GeometryUtils.h"

#include <boost/units/cmath.hpp>

using namespace trm;

LinearHeightGetter::LinearHeightGetter(Point3d p1, Point3d p2)
	: initialHeight_(p1.z())
{
	const float dist = utils::GetDistance(p1, p2);
	angle_ = Radians(std::asin((p2.z() - p1.z()) / dist));

	const Point2d p12d = Point2d::Cast(p1);

	const Line lDir = utils::GetLine(p12d, Point2d::Cast(p2));
	lNorm_ = utils::GetPerpendicularAtPoint(lDir, p12d);
}

float 
LinearHeightGetter::operator()(Point2d p) const
{
	assert(lNorm_);

	const float dist = utils::GetDistance(lNorm_.get(), p);

	return dist * boost::units::tan(angle_) + initialHeight_;
}
