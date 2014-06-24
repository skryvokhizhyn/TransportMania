#include "RailRoad.h"

using namespace trm;

const AxisType RailRoad::RAIL_ROAD_WIDTH = 2.0f;
const AxisType RailRoad::RAIL_ROAD_BORDER = 0.3f;
const AxisType RailRoad::RAIL_ROAD_STEP = 0.8f;

AxisType
RailRoad::GetTotalWidth()
{
	return RAIL_ROAD_BORDER * 2 + RAIL_ROAD_WIDTH;
}