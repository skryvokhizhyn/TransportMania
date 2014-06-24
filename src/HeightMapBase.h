#ifndef _HEIGHTMAPBASE_H_
#define _HEIGHTMAPBASE_H_

//#include "Point2d.h"

namespace trm
{
	struct Point2d;
	
namespace terrain
{
	struct HeightMapBase
	{
		typedef float Type;

		virtual size_t GetSize() const = 0;
		virtual Type At(const Point2d & p) const = 0;
		virtual void Set(const Point2d & p, const Type z) = 0;
	};

} // namespace terrain
} // namespace trm

#endif // _HEIGHTMAPBASE_H_