#ifndef _HEIGHTMAPEXTENDED_H_
#define _HEIGHTMAPEXTENDED_H_

#include "HeightMap.h"

namespace trm
{
namespace terrain
{
	class HeightMapExtended
		: public HeightMap
	{
	public:
		HeightMapExtended(const size_t sz);

		virtual Type At(const Point2d & p) const;
		virtual void Set(const Point2d & p, const Type z);

	private:
		const size_t extendedSz_;
	};

} // terrain
} // namespace trm

#endif //_HEIGHTMAPEXTENDED_H_