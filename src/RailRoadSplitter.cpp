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
		const Point2d shiftedStart = start2d - center;

		const float radiiFromSplit = shiftedSplit.GetLength();
		const float radiiFromStart = shiftedStart.GetLength();

		if (!utils::CheckNear(radiiFromSplit, radiiFromStart, 0.0001f))
		{
			return;
		}

		const Angle angleToSplit = utils::GetRotationAngle360(shiftedStart, shiftedSplit, rra.GetRotation());

		if (angleToSplit > rra.GetAngle())
		{
			return;
		}

		assert(angleToSplit >= Degrees(0) && rra.GetAngle() >= Degrees(0));

		sortedPoints.emplace(angleToSplit, p);
	});

	if (sortedPoints.empty())
	{
		return;
	}

	sortedPoints.emplace(rra.GetAngle(), end);

	Angle prevAngle;
	Point3d startPoint = start;

	boost::for_each(sortedPoints,
		[&](const SortedPointsMap::value_type & v)
	{
		spitResult_.push_back(RailRoadFactory::Arc(startPoint, v.first - prevAngle, rra.GetCenter(), rra.GetRotation()));
		startPoint = v.second;
		prevAngle = v.first;
	});
}
