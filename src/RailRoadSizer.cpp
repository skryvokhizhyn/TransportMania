#include "RailRoadSizer.h"
#include "RailRoadLine.h"
#include "RailRoadArc.h"
#include "GeometryUtils.h"

using namespace trm;

RailRoadSizer::RailRoadSizer()
	: length_(0.0f)
{}

void
RailRoadSizer::Visit(RailRoadLine & rrl)
{
	length_ = utils::GetDistance(rrl.GetStart(), rrl.GetEnd());
}

void
RailRoadSizer::Visit(RailRoadArc & rra)
{
	const Point2d start = Point2d::Cast(rra.GetStart());
	const float radii = utils::GetDistance(start, rra.GetCenter());

	length_ = radii * RawRadians(rra.GetAngle());
}

float 
RailRoadSizer::GetLenght() const
{
	return length_;
}
