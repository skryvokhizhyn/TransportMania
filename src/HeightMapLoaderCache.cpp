#include "HeightMapLoaderCache.h"
#include "Size2d.h"
#include "Point2d.h"
#include "HeightMap.h"

#include <stdexcept>

using namespace trm;
using namespace trm::terrain;

HeightMapLoaderCache::HeightMapLoaderCache(const size_t partSize, HeightMapLoaderPtr base)
	: partSize_(partSize)
	, basePtr_(base ? std::move(base) : throw std::runtime_error("Null base height map loader passed"))
{
}

size_t 
HeightMapLoaderCache::Size() const
{
	return basePtr_->Size();
}

void 
HeightMapLoaderCache::Get(const Point2d & pos, const size_t sz, HeightMap & hm) const
{
	//if (sz != partSize_)
	//{
	//	assert(false);
	//	throw std::runtime_error("Get: Height map cache cannot handle variadic sizes");
	//}
	//
	//const Size2d p = Size2d::Cast(pos);

	//HeightMapCache::iterator found = cache_.find(p);

	//if (found == cache_.end())
	//{
		basePtr_->Get(pos, sz, hm);
	/*}
	else
	{
		using namespace std;
		swap(hm, found->second);

		cache_.erase(found);
	}*/
}

void 
HeightMapLoaderCache::Set(const Point2d & pos, const HeightMap & hm)
{
	/*if (hm.GetSize() != partSize_)
	{
		assert(false);
		throw std::runtime_error("Set: Height map cache cannot handle variadic sizes");
	}

	HeightMap h;
	using namespace std;
	swap(h, hm);

	cache_.emplace(Size2d::Cast(pos), )
*/
	basePtr_->Set(pos, hm);
}
