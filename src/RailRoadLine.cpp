#include "RailRoadLine.h"

using namespace trm;

RailRoadLine::RailRoadLine(const Point3d & start, const Point3d & end)
	: start_(start), end_(end)
{}

const Point3d &
RailRoadLine::GetStart() const
{
	return start_;
}

const Point3d &
RailRoadLine::GetEnd() const
{
	return end_;
}

