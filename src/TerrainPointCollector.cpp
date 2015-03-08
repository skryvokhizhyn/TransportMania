#include "TerrainPointCollector.h"

using namespace trm;

bool 
TerrainPointCollector::operator () (const Point2d & p, float & height)
{
	points_.push_back(Point3d(p.x(), p.y(), height));
	
	return true;
}

const PointVector & 
TerrainPointCollector::GetPoints() const
{
	return points_;
}
