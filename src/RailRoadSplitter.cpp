#include "RailRoadSplitter.h"
#include "RailRoadArc.h"
#include "RailRoadLine.h"
#include "RailRoadFactory.h"
#include "RailRoadParametersTaker.h"
#include "GeometryUtils.h"

#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/adaptor/map.hpp>

#include <map>

using namespace trm;

RailRoadSplitter::RailRoadSplitter(const Polygon3d & p)
	: splitPoints_(p)
{}

const RailRoadSplitResult &
RailRoadSplitter::GetSplitResult() const
{
	return spitResult_;
}

void 
RailRoadSplitter::Visit(RailRoadLine & rrl)
{
	const Point3d & start = rrl.GetStart();
	const Point3d & end = rrl.GetEnd();

	std::map<float, Point3d> sortedPoints;

	const float lineLen = (start - end).GetLength() + 0.0001f;

	boost::for_each(splitPoints_,
		[&](const Point3d & p)
	{
		// no need to split road
		if (start == p || end == p)
		{
			return;
		}

		const float lenFromStart = (start - p).GetLength();

		// split point is outside of the line
		if (lenFromStart + (end - p).GetLength() > lineLen)
		{
			return;
		}	

		sortedPoints.emplace(lenFromStart, p);
	});

	if (sortedPoints.empty())
	{
		return;
	}

	sortedPoints.emplace(lineLen, end);

	Point3d pointFrom = start;

	boost::for_each(sortedPoints | boost::adaptors::map_values,
		[&](const Point3d & p)
	{
		spitResult_.push_back(RailRoadFactory::Line(pointFrom, p));
		pointFrom = p;
	});
}

void 
RailRoadSplitter::Visit(RailRoadArc & rra)
{
	RailRoadParametersTaker rrpt;
	rra.Accept(rrpt);

	const Point3d start = rrpt.GetStart();
	const Point2d start2d = Point2d::Cast(start);
	const Point3d & end = rrpt.GetEnd();
	const Point2d & center = rra.GetCenter();
	const Rotation rotation = utils::GetAngleRotation(rra.GetAngle());
	const Angle absAngle = utils::GetAngleAbs(rra.GetAngle());
	const Point2d shiftedStart = start2d - center;
	const float radiiFromStart = shiftedStart.GetLength();

	using SortedPointsMap = std::map<Angle, Point3d>;
	SortedPointsMap sortedPoints;

	boost::for_each(splitPoints_,
		[&](const Point3d & p)
	{
		if (start == p || end == p)
		{
			return;
		}

		const Point2d shiftedSplit = Point2d::Cast(p) - center;
		const float radiiFromSplit = shiftedSplit.GetLength();
	
		// ignore split point not on the arc
		if (!utils::CheckNear(radiiFromSplit, radiiFromStart, 0.0001f))
		{
			return;
		}

		const Angle angleToSplit = utils::GetRotationAngle360(shiftedStart, shiftedSplit, rotation);

		if (angleToSplit > absAngle)
		{
			return;
		}

		sortedPoints.emplace(angleToSplit, p);
	});

	// if no points did a split, then nothing to return
	// ignore points not on the arc and points outside of the arc
	if (sortedPoints.empty())
	{
		return;
	}

	sortedPoints.emplace(absAngle, end);

	Angle prevAngle;
	Point3d startPoint = start;

	boost::for_each(sortedPoints,
		[&](const SortedPointsMap::value_type & v)
	{
		spitResult_.push_back(RailRoadFactory::Arc(startPoint, utils::GetAdjustedAngleByRotation(v.first - prevAngle, rotation), rra.GetCenter()));
		startPoint = v.second;
		prevAngle = v.first;
	});
}
