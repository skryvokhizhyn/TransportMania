#include "RailRoadConnector.h"

#include "Point3d.h"
#include "GeometryUtils.h"
#include "RailRoadFactory.h"

#include "RailRoadArc.h"
#include "RailRoadLine.h"

#include "RailRoadTangent.h"
#include "RailRoadClosestPoint.h"
#include "RailRoadConnectionResult.h"
#include "LinearHeightGetter.h"

#include "Line.h"

#include <boost/units/cmath.hpp>

using namespace trm;

namespace
{
	const Angle ACCEPTABLE_TOLERANCE_ANGLE = Degrees(5);

	Point3d AdjustPoint(const Point3d & point, const RailRoadPtr & rrp)
	{
		if (rrp)
		{
			RailRoadClosestPoint rrcp(Point2d::Cast(point), true, 1.0f);
			rrp->Accept(rrcp);

			return rrcp.GetPoint();
		}

		return point;
	}

	OptionalPoint3d GetTangentData(const RailRoadPtr & rrp, const Point2d & near, const Point2d & far)
	{
		if (!rrp)
		{
			return OptionalPoint3d();
		}

		RailRoadTangent rrt(near, far, Degrees(60));
		rrp->Accept(rrt);

		return rrt.GetTangentVector();
	}

	RailRoadPtr ProcessOneNone(const Point3d & pointFrom, const Point3d & direction, const Point3d & pointNone)
	{
		assert (direction != Point3d());

		const PointWithDirection2d pointDirFrom = { Point2d::Cast(pointFrom), Point2d::Cast(direction) };
		const PointWithDirection2d pointDirTo = { Point2d::Cast(pointNone), Point2d::Cast(pointFrom) - Point2d::Cast(pointNone) };

		// check for same line
		if (utils::CheckPointsOnLine(pointDirFrom, pointDirTo, ACCEPTABLE_TOLERANCE_ANGLE))
		{
			return RailRoadFactory::Line(pointFrom, pointNone);
		}
		// check for same line though codirectional (cannot build arc)
		else if (Codirection::Same == utils::CheckCodirectionalWithinTolerance(pointDirFrom.direction, pointDirTo.direction, ACCEPTABLE_TOLERANCE_ANGLE))
		{
			return RailRoadPtr();
		}
		// all other cases covers Arc
		else
		{
			return RailRoadFactory::Arc(pointFrom, Point2d::Cast(direction), pointNone);
		}
	}

	RailRoadPtrs ProcessBoth(const Point3d & pointFrom, const Point3d & directionFrom, 
		const Point3d & pointTo, const Point3d & directionTo)
	{
		RailRoadPtrs result;

		const PointWithDirection2d pointDirFrom = { Point2d::Cast(pointFrom), Point2d::Cast(directionFrom) };
		const PointWithDirection2d pointDirTo = { Point2d::Cast(pointTo), Point2d::Cast(directionTo) };
		const Point2d dirFromTo = pointDirTo.point - pointDirFrom.point;
		const Codirection pointFromCodirection = utils::CheckCodirectionalWithinTolerance(pointDirFrom.direction, dirFromTo, ACCEPTABLE_TOLERANCE_ANGLE);
		const Codirection pointToCodirection = utils::CheckCodirectionalWithinTolerance(pointDirTo.direction, dirFromTo * -1.0f, ACCEPTABLE_TOLERANCE_ANGLE);

		if (pointFromCodirection == pointToCodirection && pointFromCodirection == Codirection::Same)
		{
			result.push_back(RailRoadFactory::Line(pointFrom, pointTo));
		}
		else if (Codirection::None != pointFromCodirection || Codirection::None != pointToCodirection )
		{
			// do nothing
		}
		else if (utils::CheckPointsOnCircle(pointDirFrom, pointDirTo, ACCEPTABLE_TOLERANCE_ANGLE))
		{
			result.push_back(RailRoadFactory::Arc(pointFrom, Point2d::Cast(directionFrom), pointTo));
		}
		// draw with 2 arcs
		else
		{
			const Point2d midPoint2d = RailRoadConnector::GetArcsConnectionPoint(pointDirFrom, pointDirTo);
			Point3d midPoint = Point3d::Cast(midPoint2d);
			midPoint.z() = LinearHeightGetter(pointFrom, pointTo)(midPoint2d);

			result.push_back(RailRoadFactory::Arc(pointFrom, Point2d::Cast(directionFrom), midPoint));
			result.push_back(RailRoadFactory::Arc(pointTo, Point2d::Cast(directionTo), midPoint));
		}

		return result;
	}
}

TangentPointPair 
RailRoadConnector::GetRoadTangents(const Point3d & pLeft, const RailRoadPtr & rrpLeft, const Point3d & pRight, const RailRoadPtr & rrpRight)
{
	const Point3d leftAdjusted = AdjustPoint(pLeft, rrpLeft);
	const Point3d rightAdjusted = AdjustPoint(pRight, rrpRight);

	TangentPointPair result;

	const Point2d pLeft2d = Point2d::Cast(leftAdjusted);
	const Point2d pRight2d = Point2d::Cast(rightAdjusted);

	const OptionalPoint3d leftTangent = GetTangentData(rrpLeft, pLeft2d, pRight2d);
	const OptionalPoint3d rightTangent = GetTangentData(rrpRight, pRight2d, pLeft2d);

	// we roll back to the initial point if no tangent has been found
	result.first = {leftTangent ? leftAdjusted : pLeft, leftTangent};
	result.second = {rightTangent ? rightAdjusted : pRight, rightTangent};
	
	return result;
}

RailRoadPtrs 
RailRoadConnector::GetRoads(const TangentPointPair & tangents)
{
	RailRoadPtrs result;

	const auto & firstTangent = tangents.first;
	const auto & secondTangent = tangents.second;

	if (!firstTangent.direction && !secondTangent.direction)
	{
		result.push_back(
			RailRoadFactory::Line(firstTangent.point, secondTangent.point));
	}
	else if (firstTangent.direction && !secondTangent.direction)
	{
		const auto road = ProcessOneNone(firstTangent.point, firstTangent.direction.get(), secondTangent.point);
		if (road)
		{
			result.push_back(road);
		}
	}
	else if (!firstTangent.direction && secondTangent.direction)
	{
		const auto road = ProcessOneNone(secondTangent.point, secondTangent.direction.get(), firstTangent.point);
		if (road)
		{
			result.push_back(road);
		}
	}
	else
	{
		RailRoadPtrs intermediateRoads = ProcessBoth(firstTangent.point, firstTangent.direction.get(), secondTangent.point, secondTangent.direction.get());
		result.insert(result.end(), intermediateRoads.begin(), intermediateRoads.end());
	}

	return result;
}

RailRoadConnectionResult 
RailRoadConnector::GetRoads(Point3d pLeft, const RailRoadPtr & rrpLeft, Point3d pRight, const RailRoadPtr & rrpRight)
{
	const TangentPointPair tangentPair = GetRoadTangents(pLeft, rrpLeft, pRight, rrpRight);

	RailRoadConnectionResult result;
	result.roadPtrs = GetRoads(tangentPair);

	if (tangentPair.first.direction)
	{
		result.intersestions.emplace_back(rrpLeft, tangentPair.first.point);
	}

	if (tangentPair.second.direction)
	{
		result.intersestions.emplace_back(rrpRight, tangentPair.second.point);
	}

	return result;
} 

Point2d 
RailRoadConnector::GetArcsConnectionPoint(const PointWithDirection2d & pointDirFrom, const PointWithDirection2d & pointDirTo)
{
	Point2d dirFromTo = pointDirTo.point - pointDirFrom.point;

	const Angle angleBetweenDirFromToAndDirFrom = utils::GetSignedAngle180(dirFromTo, pointDirFrom.direction);
	const Point2d dirMiddleFrom = utils::RotateVector(dirFromTo, angleBetweenDirFromToAndDirFrom * 0.5f);

	dirFromTo *= -1.0f;

	const Angle angleBetweenDirToFromAndDirTo = utils::GetSignedAngle180(pointDirTo.direction, dirFromTo);
	const Point2d dirMiddleTo = utils::RotateVector(pointDirTo.direction, angleBetweenDirToFromAndDirTo * 0.5f);

	if (utils::GetAngleRotation(angleBetweenDirFromToAndDirFrom) != utils::GetAngleRotation(angleBetweenDirToFromAndDirTo))
	{
		return (pointDirFrom.point + pointDirTo.point) * 0.5f;
	}

	const Line lineFrom = utils::GetLine(pointDirFrom.point, dirMiddleFrom + pointDirFrom.point);
	const Line lineTo = utils::GetLine(pointDirTo.point, dirMiddleTo + pointDirTo.point);
	
	return utils::GetIntersectionPoint(lineFrom, lineTo);
}
