#include "RailRoadFactory.h"
#include "RailRoadArc.h"
#include "RailRoadLine.h"
#include "Point3d.h"
#include "GeometryUtils.h"
#include "Line.h"

using namespace trm;

RailRoadPtr
RailRoadFactory::Line(const Point3d & s, const Point3d & e)
{
	if (s == e)
	{
		throw std::runtime_error("Cannot build Line based on zero length");
	}

	return std::make_shared<RailRoadLine>(s, e);
}

RailRoadPtr 
RailRoadFactory::Arc(const Point3d & s, const Angle a, const Point2d & c)
{
	if (a == Degrees(0))
	{
		throw std::runtime_error("Cannot build Arc based on zero angle");
	}

	if (Point2d::Cast(s) == c)
	{
		throw std::runtime_error("Cannot build Arc based on zero radii");
	}

	return std::make_shared<RailRoadArc>(Spiral3d{ s, a, c });
}

RailRoadPtr 
RailRoadFactory::Arc(const Point3d & start, const Point3d & direction, const Point2d & end)
{
	if (direction == Point3d())
	{
		throw std::runtime_error("Cannot build Arc based on zero direction vector");
	}

	const Point2d start2d = Point2d::Cast(start);

	if (start2d == end)
	{
		throw std::runtime_error("Cannot build Arc based on the same start/end points");
	}

	const Point3d dirEnd = start + direction;

	const trm::Line vecLine = utils::GetLine(start2d, Point2d::Cast(dirEnd));
	const trm::Line vecPerpendicularLine = utils::GetPerpendicularAtPoint(vecLine, start2d);
	const trm::Line chordLine = utils::GetLine(start2d, end);
	const trm::Line chordPerpendicularLine = utils::GetPerpendicularAtPoint(chordLine, (start2d + end) / 2.0f);

	const Point2d center = utils::GetIntersectionPoint(vecPerpendicularLine, chordPerpendicularLine);

	const Point2d vStart = start2d - center;
	const Point2d vEnd = Point2d::Cast(dirEnd) - center;

	const Angle rotAngle = utils::GetSignedAngle(vStart, vEnd);
	const Rotation rot = utils::GetAngleRotation(rotAngle);

	const Point2d end2dShifted = end - center;

	const Angle a = utils::GetRotationAngle360(vStart, end2dShifted, rot);

	return RailRoadFactory::Arc(start, utils::GetAdjustedAngleByRotation(a, rot), center);
}
