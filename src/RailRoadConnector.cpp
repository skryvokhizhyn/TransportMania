#include "RailRoadConnector.h"

#include "Point3d.h"
#include "GeometryUtils.h"
#include "RailRoadFactory.h"

#include "RailRoadArc.h"
#include "RailRoadLine.h"

#include "RailRoadTangent.h"
#include "RailRoadClosestPoint.h"
#include "RailRoadConnectionResult.h"

#include "Line.h"

#include <boost/units/cmath.hpp>

using namespace trm;

namespace
{
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

		const Point3d shiftedTo = pointNone - pointFrom;

		if (utils::CheckColinear(Point2d::Cast(shiftedTo), Point2d::Cast(direction)))
		{
			return RailRoadFactory::Line(pointFrom, pointNone);
		}
		else
		{
			return RailRoadFactory::Arc(pointFrom, direction, Point2d::Cast(pointNone));
		}
	}

	RailRoadPtrs ProcessBoth(const Point3d & pointFrom, const Point3d & directionFrom, 
		const Point3d & pointTo, const Point3d & directionTo)
	{
		RailRoadPtrs result;
		const static Angle lineToleranceAngle = Degrees(5);

		const Angle angleFromToDirectionFrom = utils::GetSignedAngle180(Point2d::Cast(directionFrom), Point2d::Cast(directionTo));
		
		const Point2d directionFrom2d = Point2d::Cast(directionFrom);
		const Point2d directionTo2d = Point2d::Cast(directionTo);
		const Point2d fromToDirection2d = Point2d::Cast(pointTo - pointFrom);

		namespace bu = boost::units;

		if (bu::abs(utils::GetSignedAngle(directionFrom2d, fromToDirection2d)) <= lineToleranceAngle
			&& bu::abs(utils::GetSignedAngle(directionTo2d, fromToDirection2d)) <= lineToleranceAngle)
		{
			result.push_back(RailRoadFactory::Line(pointFrom, pointTo));
		}
		else
		{
			const Angle angleBetweenDirections = utils::GetSignedAngle180(Point2d::Cast(directionFrom), Point2d::Cast(directionTo));

			if (angleBetweenDirections > Degrees(0))
			{
				result.push_back(RailRoadFactory::Arc(pointFrom, directionFrom, Point2d::Cast(pointTo)));
			}
			else if (angleBetweenDirections < Degrees(0))
			{
				const Point2d midPoint = Point2d::Cast((pointFrom + pointTo) / 2.0f);
				result.push_back(RailRoadFactory::Arc(pointFrom, directionFrom, midPoint));
				result.push_back(RailRoadFactory::Arc(pointTo, directionTo, midPoint));
			}
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
		result.push_back(
			ProcessOneNone(firstTangent.point, firstTangent.direction.get(), secondTangent.point));
	}
	else if (!firstTangent.direction && secondTangent.direction)
	{
		result.push_back(
			ProcessOneNone(secondTangent.point, secondTangent.direction.get(), firstTangent.point));
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
