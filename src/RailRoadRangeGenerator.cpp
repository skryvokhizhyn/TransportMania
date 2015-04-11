#include "RailRoadRangeGenerator.h"
#include "RailRoadLine.h"
#include "RailRoadArc.h"
#include "TerrainRangeLine.h"
#include "TerrainRangeArc.h"

using namespace trm;

void 
RailRoadRangeGenerator::Visit(RailRoadArc & rra)
{
	range_ = TerrainRangeArc(TerrainRangeArc::Data(Point2d::Cast(rra.GetStart()), rra.GetAngle(), rra.GetCenter(), rra.GetRotation()), RailRoad::GetTotalWidth());
}
		
void 
RailRoadRangeGenerator::Visit(RailRoadLine & rrl)
{
	range_ = TerrainRangeLine(Point2d::Cast(rrl.GetStart()), Point2d::Cast(rrl.GetEnd()), RailRoad::GetTotalWidth());
}

const TerrainRange &
RailRoadRangeGenerator::GetRange()
{
	return range_;
}
