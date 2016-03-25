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

	// near pos is away from line
	if (utils::GetDistance(utils::GetLine(beg, end), nearPos_) > 0.0001f)
	{
		return;
	}

	const Point2d dirVec = farPos_ - nearPos_;
	const Point2d rrlVec = end - beg;

	const Angle deg = utils::GetAngle(rrlVec, dirVec);

	// angle between vector and line is bigger than the given threshold
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
		direction_ = (utils::GetAngleAbs(deg) < invalidAngle_) ? dirLine : Point3d(dirLine * -1);
	}
}

void
RailRoadTangent::Visit(RailRoadArc & rra)
{
	const Point2d & c = rra.GetCenter();
	
	const Point2d dir = nearPos_ - c;
	const Rotation rotation = utils::GetAngleRotation(rra.GetAngle());
	
	const Point2d startShifted = Point2d::Cast(rra.GetStart()) - c;
	const Angle rotationToNearPos = utils::GetRotationAngle360(startShifted, dir, rotation);

	const Angle angleAbs = utils::GetAngleAbs(rra.GetAngle());

	// ignore near point if it's out of the arc
	if (rotationToNearPos > angleAbs)
	{
		return;
	}

	Point3d nearPos3d = Point3d::Cast(nearPos_);
	nearPos3d.z() = rra.GetStart().z() + (rra.GetZShift() * rotationToNearPos / angleAbs);

	const Angle rotationAngle = utils::GetAdjustedAngleByRotation(Degrees(90), rotation);
	const Point2d nextPoint = utils::RotateVector(dir, rotationAngle);
	
	Point3d nextPos3d = Point3d::Cast(nextPoint);
	nextPos3d.z() = rra.GetZShift() * Degrees(90) / angleAbs;

	ProcessLine(nearPos3d, nextPos3d + nearPos3d);
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

