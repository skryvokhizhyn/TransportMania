#include "RailRoadTangent.h"
#include "RailRoadArc.h"
#include "RailRoadLine.h"
#include "GeometryUtils.h"
#include "Line.h"

using namespace trm;

RailRoadTangent::RailRoadTangent(const Point2d & near, const Point2d & far, Angle invalidAngle)
	: nearPos_(near)
	, farPos_(far)
	, invalidAngle_(invalidAngle)
{}

void 
RailRoadTangent::ProcessLine(const Point3d & beg3d, const Point3d & end3d)
{
	const Point2d beg = Point2d::Cast(beg3d);
	const Point2d end = Point2d::Cast(end3d);

	const Point2d dirVec = farPos_ - nearPos_;
	const Point2d rrlVec = end - beg;

	const Angle deg = utils::GetAngle(rrlVec, dirVec);

	if (deg >= invalidAngle_ && deg <= Degrees(180) - invalidAngle_)
	{
		return;
	}

	const Point3d dirLine = end3d - beg3d;

	if (deg == Degrees(0))
	{
		direction_ = dirLine;
	}
	else if (deg == Degrees(180))
	{
		direction_ = -1 * dirLine;
	}
	else
	{
		direction_ = (deg < invalidAngle_) ? dirLine : Point3d(dirLine * -1);
	}
}

void
RailRoadTangent::Visit(RailRoadArc & rra)
{
	const Point2d & c = rra.GetCenter();
	Point3d c3d = Point3d::Cast(c);
	Point3d n3d = Point3d::Cast(nearPos_);
	n3d.z() = rra.GetStart().z();
	c3d.z() = n3d.z();

	const Point3d dir = c3d - n3d;
	const Point2d nextPoint = utils::RotateVector(Point2d::Cast(dir), Degrees(90), rra.GetRotation());
	Point3d nextPoint3d = Point3d::Cast(nextPoint);
	nextPoint3d.z() = n3d.z();

	ProcessLine(nextPoint3d + n3d, n3d);
}

void
RailRoadTangent::Visit(RailRoadLine & rrl)
{
	ProcessLine(rrl.GetStart(), rrl.GetEnd());
}

const OptionalPoint3d & 
RailRoadTangent::GetTangentVector() const
{
	return direction_;
}

