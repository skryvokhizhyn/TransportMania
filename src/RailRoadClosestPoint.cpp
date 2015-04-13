#include "RailRoadClosestPoint.h"
#include "Line.h"
#include "GeometryUtils.h"
#include "RailRoadLine.h"
#include "RailRoadArc.h"

using namespace trm;

RailRoadClosestPoint::RailRoadClosestPoint(const Point2d & p, bool stickToBorders)
	: currentPos_(p)
	, stickToBorders_(stickToBorders)
{}

void 
RailRoadClosestPoint::Visit(RailRoadArc & rra)
{
	const Point2d & center = rra.GetCenter();

	const Point2d shiftedCurrentPos = currentPos_ - center;
	const float distToPos = shiftedCurrentPos.GetLength();
	
	const Point2d start2d = Point2d::Cast(rra.GetStart());

	if (distToPos < 0.0001f)
	{
		foundPos_ = start2d;
		return;
	}

	const float radii = (start2d - center).GetLength();

	const Point2d posOnArc = shiftedCurrentPos * radii / distToPos;

	if (!stickToBorders_)
	{
		foundPos_ = posOnArc + center;
		return;
	}
	
	const Angle strictAngle = utils::GetRotationAngle360(start2d, posOnArc, rra.GetRotation());

	const Angle & angle = rra.GetAngle();

	if (strictAngle < angle)
	{
		foundPos_ = posOnArc + center;
		return;
	}

	const Angle extraAngle = strictAngle - angle;
	const Angle overAngle = Degrees(360) - angle;

	if (extraAngle >= overAngle / 2.0f)
	{
		foundPos_ = start2d;
	}
	else
	{
		const Point2d endPoint = utils::RotateVector(start2d, angle, rra.GetRotation());
		foundPos_ = endPoint + center;
	}
}

void 
RailRoadClosestPoint::Visit(RailRoadLine & rrl)
{
	const Point2d pStart = Point2d::Cast(rrl.GetStart());
	const Point2d pEnd = Point2d::Cast(rrl.GetEnd());

	static const float allowedPrecision = 0.00001f;

	if ((currentPos_ - pStart).GetLength() < allowedPrecision)
	{
		foundPos_ = pStart;
		return;
	}

	if ((currentPos_ - pEnd).GetLength() < allowedPrecision)
	{
		foundPos_ = pEnd;
		return;
	}

	const Line line = utils::GetLine(pStart, pEnd);
	const Line perp = utils::GetPerpendicularAtPoint(line, currentPos_);

	foundPos_ = utils::GetIntersectionPoint(line, perp);

	if (!stickToBorders_)
	{
		return;
	}

	const Point2d dirStart = pStart - foundPos_;
	const Point2d dirEnd = pEnd - foundPos_;

	const float dirStartLen = dirStart.GetLength();
	const float dirEndLen = dirEnd.GetLength();
	const float dirRoad = (pEnd - pStart).GetLength();
	
	// if summ of the dists is bigger than the whole dist
	if (dirStartLen + dirEndLen > dirRoad && dirStartLen < dirRoad && dirEndLen < dirRoad)
	{
		if (dirStartLen > dirEndLen)
		{
			foundPos_ = pEnd;
		}
		else
		{
			foundPos_ = pStart;
		}
	}
}

const Point2d &
RailRoadClosestPoint::GetPoint() const
{
	return foundPos_;
}
