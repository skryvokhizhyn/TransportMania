#include "RailRoadMover.h"
#include "RailRoadLine.h"
#include "RailRoadArc.h"
#include "GeometryUtils.h"

using namespace trm;

RailRoadMover::RailRoadMover(Point3d p, const float dist)
	: dist_(dist), pos_(p)
{}

void 
RailRoadMover::Visit(RailRoadArc & rra)
{
	const Point2d start = Point2d::Cast(rra.GetStart());
	const Point2d & center = rra.GetCenter();

	const float radii = utils::GetDistance(start, center);

	const Angle a = Radians(dist_ / radii);

	Point2d v = Point2d::Cast(pos_) - rra.GetCenter();
	v = utils::RotateVector(v, a, rra.GetRotation());
	Point3d pos = Point3d::Cast(v + rra.GetCenter());
	pos.z() = pos_.z();

	pos_ = pos;
}

void 
RailRoadMover::Visit(RailRoadLine & rrl)
{
	Point3d norm = rrl.GetEnd() - rrl.GetStart();
	norm.Normalize();

	pos_ += norm * dist_;
}

const Point3d & 
RailRoadMover::GetPosition() const
{
	return pos_;
}