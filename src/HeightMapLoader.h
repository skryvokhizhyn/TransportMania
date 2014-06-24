#ifndef _HEIGHTMAPLOADER_H_
#define _HEIGHTMAPLOADER_H_

//#include "Point2d.h"
#include <memory>

namespace trm
{
	struct Point2d;

namespace terrain
{
	class HeightMap;

	struct HeightMapLoader
	{
		virtual size_t getSize() const = 0;
		virtual void Get(const Point2d & pos, const size_t sz, HeightMap & hm) = 0;
		virtual void Set(const Point2d & pos, const HeightMap & hm) = 0;
	};

	typedef std::shared_ptr<HeightMapLoader> HeightMapLoaderPtr;

} // namespace terrain
} // namespace trm

#endif // _HEIGHTMAPLOADER_H_