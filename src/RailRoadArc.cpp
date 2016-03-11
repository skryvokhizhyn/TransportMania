#include "RailRoadArc.h"
#include "GeometryUtils.h"
#include "ModelData.h"

using namespace trm;

RailRoadArc::RailRoadArc(const Point3d & s, const Angle a, const Point2d & c)
	: start_(s), angle_(a), center_(c)
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

	assert(angle_ != Degrees(0));
}

const Point3d & 
RailRoadArc::GetStart() const
{
	return start_;
}

Angle 
RailRoadArc::GetAngle() const
{
	return angle_;
}

const Point2d & 
RailRoadArc::GetCenter() const
{
	return center_;
}
