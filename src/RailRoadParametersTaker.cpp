#include "RailRoadParametersTaker.h"
#include "RailRoadLine.h"
#include "RailRoadArc.h"
#include "GeometryUtils.h"

using namespace trm;

void
RailRoadParametersTaker::Visit(RailRoadLine & rrl)
{
	start_ = rrl.GetStart();
	end_ = rrl.GetEnd();
}

void
RailRoadParametersTaker::Visit(RailRoadArc & rra)
{
	start_ = rra.GetStart();

	const Point2d end2d = utils::RotateVector(Point2d::Cast(start_) - rra.GetCenter(), rra.GetAngle());

	end_ = Point3d::Cast(end2d + rra.GetCenter());
	end_.z() = start_.z() + rra.GetZShift();
}

const Point3d & 
RailRoadParametersTaker::GetStart() const
{
	return start_;
}

const Point3d & 
RailRoadParametersTaker::GetEnd() const
{
	return end_;
}
