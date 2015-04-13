#include "TerrainHeightGetter.h"

using namespace trm;

bool 
TerrainHeightGetter::operator () (const Point2d & p, float & height)
{
	Point3d p3d = Point3d::Cast(p);
	p3d.z() = height;

	points_.push_back(p3d);

	return true;
}

const Polygon3d & 
TerrainHeightGetter::Get() const
{
	return points_;
}
