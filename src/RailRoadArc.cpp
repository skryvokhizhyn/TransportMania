#include "RailRoadArc.h"
#include "GeometryUtils.h"
#include "ModelData.h"

using namespace trm;

RailRoadArc::RailRoadArc(const Spiral3d & data)
	: data_(data)
{
	/*const AxisType radii = utils::GetDistance(Point2d::Cast(s), c);
	
	if (radii < RAIL_ROAD_WIDTH / 2)
	{
		const auto msg = (boost::format("Trying to build Railroad Arc with too small radii. %1% < %2%") 
			% radii % (RAIL_ROAD_WIDTH / 2));

		throw std::runtime_error(msg.str());
	}

	if (radii < RAIL_ROAD_STEP)
	{
		const auto msg = (boost::format("Trying to build Railroad Arc with too small radii. %1% < %2%") 
			% radii % RAIL_ROAD_STEP);

		throw std::runtime_error(msg.str());
	}*/

	assert(data_.angle != Degrees(0));
}

const Point3d & 
RailRoadArc::GetStart() const
{
	return data_.start;
}

Angle 
RailRoadArc::GetAngle() const
{
	return data_.angle;
}

const Point2d & 
RailRoadArc::GetCenter() const
{
	return data_.center;
}
