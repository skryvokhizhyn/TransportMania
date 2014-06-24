#ifndef _TERRAINRANGEARC_H_
#define _TERRAINRANGEARC_H_

#include "TerrainRange.h"
#include "Point2d.h"
#include "Angle.h"
#include "Types.h"

namespace trm
{
	class TerrainRangeArc
		: public TerrainRange
	{
	public:
		struct Data
			: boost::noncopyable
		{
			Data(const Point2d & s, const Angle a, const Point2d & c, const Direction d);

			const Point2d start;
			const Angle angle;
			const Point2d center;
			const Direction dir;
		};
		
	public:
		TerrainRangeArc(const Data & data, AxisType width);

	public:
		typedef std::vector<AxisPairType> IntersectionType;
		static IntersectionType GetIntersection(Point2d vec, const Angle a, Direction d, const int y, const AxisPairType & p);

	private:
		void ProcessRange(const Point2d & vec, const Angle a, const Direction dir, const int y, const AxisPairType & pt, const Point2d & c);
	};

} // namespace trm

#endif // _TERRAINRANGEARC_H_