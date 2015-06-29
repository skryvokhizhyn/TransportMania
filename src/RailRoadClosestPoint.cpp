#include "RailRoadClosestPoint.h"
#include "Line.h"
#include "GeometryUtils.h"
#include "RailRoadLine.h"
#include "RailRoadArc.h"

using namespace trm;

RailRoadClosestPoint::RailRoadClosestPoint(const Point2d & p, bool stickToBorders, float bordersTolerance)
	: currentPos_(p)
	, stickToBorders_(stickToBorders)
	, bordersTolerance_(bordersTolerance)
{}

void 
RailRoadClosestPoint::Visit(RailRoadArc & rra)
{
	const Point2d & center = rra.GetCenter();
	const Point3d & start3d = rra.GetStart();

	const Point2d shiftedCurrentPos = currentPos_ - center;
	const float distToPos = shiftedCurrentPos.GetLength();

	const Point2d start2d = Point2d::Cast(start3d);

	if (distToPos < bordersTolerance_)
	{
		foundPos_ = start3d;
		return;
	}

	const float radii = (start2d - center).GetLength();

	const Point2d posOnArc = shiftedCurrentPos * radii / distToPos;

	if (!stickToBorders_)
	{
		foundPos_ = Point3d::Cast(posOnArc + center);
		foundPos_.z() = start3d.z();
		return;
	}

	const Angle strictAngle = utils::GetRotationAngle360(start2d, posOnArc, rra.GetRotation());

	const Angle & angle = rra.GetAngle();

	if (strictAngle < angle)
	{
		foundPos_ = Point3d::Cast(posOnArc + center);
		foundPos_.z() = start3d.z();
		return;
	}
	
	const Angle extraAngle = strictAngle - angle;
	const Angle overAngle = Degrees(360) - angle;

	if (extraAngle >= overAngle / 2.0f)
	{
		foundPos_ = start3d;
	}
	else
	{
		const Point2d endPoint = utils::RotateVector(start2d, angle, rra.GetRotation());
		foundPos_ = Point3d::Cast(endPoint + center);
		foundPos_.z() = start3d.z();
	}
}

void 
RailRoadClosestPoint::Visit(RailRoadLine & rrl)
{
	const Point3d & pStart3d = rrl.GetStart();
	const Point2d pStart = Point2d::Cast(pStart3d);
	const Point3d & pEnd3d = rrl.GetEnd();
	const Point2d pEnd = Point2d::Cast(pEnd3d);

	if ((currentPos_ - pStart).GetLength() < bordersTolerance_)
	{
		foundPos_ = pStart3d;
		return;
	}

	if ((currentPos_ - pEnd).GetLength() < bordersTolerance_)
	{
		foundPos_ = pEnd3d;
		return;
	}

	const Line line = utils::GetLine(pStart, pEnd);
	const Line perp = utils::GetPerpendicularAtPoint(line, currentPos_);

	const Point2d intersectionPos = utils::GetIntersectionPoint(line, perp);

	const Point2d dirStart = pStart - intersectionPos;
	const Point2d dirEnd = pEnd - intersectionPos;

	const float dirStartLen = dirStart.GetLength();
	const float dirEndLen = dirEnd.GetLength();
	const float dirRoad = (pEnd - pStart).GetLength();
	
	if (dirStartLen + dirEndLen <= dirRoad)
	{
		foundPos_ = Point3d::Cast(intersectionPos);
		foundPos_.z() = pStart3d.z() + (pEnd3d.z() - pStart3d.z()) * dirStartLen / dirRoad;

		return;
	}

	if (!stickToBorders_)
	{
		foundPos_ = Point3d::Cast(intersectionPos);

		if (dirStartLen < dirEndLen)
		{
			foundPos_.z() = pStart3d.z() - (pEnd3d.z() - pStart3d.z()) * dirStartLen / dirRoad;
		}
		else
		{
			foundPos_.z() = pEnd3d.z() + (pEnd3d.z() - pStart3d.z()) * dirEndLen / dirRoad;
		}

		return;
	}

	// if summ of the dists is bigger than the whole dist
	if (dirStartLen > dirEndLen)
	{
		foundPos_ = pEnd3d;
	}
	else
	{
		foundPos_ = pStart3d;
	}
}

const Point3d &
RailRoadClosestPoint::GetPoint() const
{
	return foundPos_;
}
