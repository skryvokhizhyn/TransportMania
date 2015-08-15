#include "RailRoad.h"

using namespace trm;

namespace
{
	const AxisType RAIL_ROAD_WIDTH = 2.0f;
	const AxisType RAIL_ROAD_BORDER = 0.3f;
	const AxisType RAIL_ROAD_STEP = 0.8f;
	const AxisType RAIL_ROAD_Z_SHIFT = 0.01f;
}

AxisType
RailRoad::GetTotalWidth()
{
	return RAIL_ROAD_BORDER * 2 + RAIL_ROAD_WIDTH;
}

AxisType 
RailRoad::GetRoadWidth()
{
	return RAIL_ROAD_WIDTH;
}

AxisType 
RailRoad::GetRoadStep()
{
	return RAIL_ROAD_STEP;
}

AxisType 
RailRoad::GetRoadZShift()
{
	return RAIL_ROAD_Z_SHIFT;
}
