#include "RailRoadSplitter.h"
#include "RailRoadArc.h"
#include "RailRoadLine.h"
#include "RailRoadFactory.h"

using namespace trm;

RailRoadSplitter::RailRoadSplitter(const Point3d & p)
	: splitPoint_(p)
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

	// no need to split road
	if (start == splitPoint_ || end == splitPoint_)
	{
		return;
	}

	// split point is outside of the line
	if ((start - splitPoint_).GetLength() +
		(end - splitPoint_).GetLength() > (start - end).GetLength() + 0.0001f)
	{
		return;
	}

	spitResult_ = RailRoadSplitResult(std::make_pair(
		RailRoadFactory::Line(start, splitPoint_),
		RailRoadFactory::Line(splitPoint_, end)));
}

#include "RailRoadParametersTaker.h"
#include "GeometryUtils.h"

void 
RailRoadSplitter::Visit(RailRoadArc & rra)
{
	RailRoadParametersTaker rrpt;
	rra.Accept(rrpt);

	if (rrpt.GetStart() == splitPoint_ ||
		rrpt.GetEnd() == splitPoint_)
	{
		return;
	}

	const Point2d shiftedSplit = Point2d::Cast(splitPoint_) - rra.GetCenter();
	const Point2d shiftedStart = Point2d::Cast(rra.GetStart()) - rra.GetCenter();

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

	spitResult_ = RailRoadSplitResult(std::make_pair(
		RailRoadFactory::Arc(rrpt.GetStart(), angleToSplit, rra.GetCenter(), rra.GetRotation()),
		RailRoadFactory::Arc(splitPoint_, rra.GetAngle() - angleToSplit, rra.GetCenter(), rra.GetRotation())));
}
