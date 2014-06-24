#ifndef _TERRAINRANGECIRCLE_H_
#define _TERRAINRANGECIRCLE_H_

#include "TerrainRange.h"
#include "Types.h"

namespace trm
{
	class TerrainRangeCircle
		: public TerrainRange
	{
	public:
		TerrainRangeCircle(const Point2d & center, const AxisType radii);
	};

} // namespace trm

#endif // _TERRAINRANGECIRCLE_H_