#ifndef _TERRAINRANGEARC_H_
#define _TERRAINRANGEARC_H_

#include "TerrainRange.h"
#include "Point2d.h"
#include "Angle.h"
#include "Types.h"
#include "Rotation.h"

namespace trm
{
	class TerrainRangeArc
		: public TerrainRange
	{
	public:
		struct Data
			: boost::noncopyable
		{
			Data(const Point2d & s, const Angle a, const Point2d & c);

			const Point2d start;
			const Angle angle;
			const Point2d center;
		};
		
	public:
		TerrainRangeArc(const Data & data, AxisType width);

	public:
		typedef std::vector<AxisPairType> IntersectionType;
		static IntersectionType GetIntersection(Point2d vec, const Angle a, const int y, const AxisPairType & p);

	private:
		void ProcessRange(const Point2d & vec, Angle a, const int y, const AxisPairType & pt, const Point2d & c);
	};

} // namespace trm

#endif // _TERRAINRANGEARC_H_