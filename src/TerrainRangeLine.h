#ifndef _TERRAINRANGELINE_H_
#define _TERRAINRANGELINE_H_

#include "TerrainRange.h"

namespace trm
{
	class TerrainRangeLine
		: public TerrainRange
	{
	public:
		TerrainRangeLine(const Point2d & start, const Point2d & end, const AxisType width);
	};

} // namespace trm

#endif // _TERRAINRANGELINE_H_