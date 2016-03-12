#ifndef _TERRAINRANGEARC_H_
#define _TERRAINRANGEARC_H_

#include "TerrainRange.h"
#include "Arc2d.h"
#include "Types.h"
#include "Rotation.h"

namespace trm
{
	class TerrainRangeArc
		: public TerrainRange
	{
	public:
		TerrainRangeArc(const Arc2d & data, AxisType width);

	public:
		typedef std::vector<AxisPairType> IntersectionType;
		static IntersectionType GetIntersection(Point2d vec, const Angle a, const int y, const AxisPairType & p);

	private:
		void ProcessRange(const Point2d & vec, Angle a, const int y, const AxisPairType & pt, const Point2d & c);
	};

} // namespace trm

#endif // _TERRAINRANGEARC_H_