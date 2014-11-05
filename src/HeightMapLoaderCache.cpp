#include "HeightMapLoaderCache.h"
#include "Size2d.h"
#include "Point2d.h"

#include <stdexcept>

using namespace trm;
using namespace trm::terrain;

namespace
{
	bool IsCornerPoint(const Size2d & p, size_t partSize)
	{
		--partSize;

		return (p.x() % partSize == 0) && (p.y() % partSize == 0);
	}
}

HeightMapLoaderCache::HeightMapLoaderCache(const size_t partSize, HeightMapLoaderPtr base)
	: partSize_(partSize)
	, basePtr_(base ? std::move(base) : throw std::runtime_error("Null base height map loader passed"))
	, wholeSize_(basePtr_->Size())
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
	Size2d point = Size2d::Cast(pos);

	// special case of patch corner request
	if (sz == 1)
	{
		if (point.x() == (partSize_ - 1))
			--point.x();
		if (point.y() == (partSize_ - 1))
			--point.y();

		Size2d cornerPos = point / (partSize_ - 1);
		cornerPos *= (partSize_ - 1);

		const auto patchFound = heightMapCache_.find(cornerPos);

		if (patchFound == heightMapCache_.end())
		{
			basePtr_->Get(pos, sz, hm);
		}
		else
		{
			HeightMap::Container vec;
			HeightMap::Value val = HeightMap::GetValueAt(pos - Point2d::Cast(cornerPos), patchFound->second, partSize_);
			vec.push_back(val);

			hm.Swap(vec);
		}
	}
	else
	{
		if (sz != partSize_)
		{
			throw std::runtime_error("Sizes other than part size aren't supported by height map cache (Get)");
		}
		
		HeightMapCache::iterator found = heightMapCache_.find(point);

		if (found == heightMapCache_.end())
		{
			basePtr_->Get(pos, sz, hm);
		}
		else
		{
			HeightMap::Container hmc = found->second;
			
			hm.Swap(hmc);
		}
	}
}

void 
HeightMapLoaderCache::Set(const Point2d & pos, const HeightMap & hm)
{
	if (hm.GetSize() != partSize_)
	{
		throw std::runtime_error("Sizes other than part size aren't supported by height map cache (Set)");
	}

	const Size2d p = Size2d::Cast(pos);
	if (!IsCornerPoint(p, partSize_))
	{
		throw std::runtime_error("Non corner points aren't supported by height map cache (Set)");
	}

	heightMapCache_[p] = hm.GetData();

	//basePtr_->Set(pos, hm);
}
