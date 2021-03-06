#include "RailRoadRangeGenerator.h"
#include "RailRoadLine.h"
#include "RailRoadArc.h"
#include "TerrainRangeLine.h"
#include "TerrainRangeArc.h"

using namespace trm;

RailRoadRangeGenerator::RailRoadRangeGenerator(bool useFullWidth)
	: useFullWidth_(useFullWidth)
{}

void 
RailRoadRangeGenerator::Visit(RailRoadArc & rra)
{
	range_ = TerrainRangeArc({ Point2d::Cast(rra.GetStart()), rra.GetAngle(), rra.GetCenter() },
		useFullWidth_ ? RailRoad::GetTotalWidth() : RailRoad::GetRoadWidth());
}
		
void 
RailRoadRangeGenerator::Visit(RailRoadLine & rrl)
{
	range_ = TerrainRangeLine(Point2d::Cast(rrl.GetStart()), Point2d::Cast(rrl.GetEnd()), 
		useFullWidth_ ? RailRoad::GetTotalWidth() : RailRoad::GetRoadWidth());
}

const TerrainRange &
RailRoadRangeGenerator::GetRange()
{
	return range_;
}
