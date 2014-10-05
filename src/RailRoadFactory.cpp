#include "RailRoadFactory.h"
#include "RailRoadArc.h"
#include "RailRoadLine.h"
#include "Point3d.h"

using namespace trm;

RailRoadPtr
RailRoadFactory::Line(const Point3d & s, const Point3d & e)
{
	return std::make_shared<RailRoadLine>(s, e);
}

RailRoadPtr 
RailRoadFactory::Arc(const Point3d & s, const Angle a, const Point2d & c, const Rotation r)
{
	return std::make_shared<RailRoadArc>(s, a, c, r);
}