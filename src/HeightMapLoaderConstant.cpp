#include "HeightMapLoaderConstant.h"
#include "HeightMap.h"
#include "Point2d.h"

using namespace trm;
using namespace trm::terrain;

HeightMapLoaderConstant::HeightMapLoaderConstant(const size_t totalSize, const float patchV, const float pointV, const float increment)
	: totalSize_(totalSize)
	, patchValue_(patchV)
	, pointValue_(pointV)
	, increment_(increment)
{
}

size_t 
HeightMapLoaderConstant::Size() const
{
	return totalSize_;
}

void 
HeightMapLoaderConstant::Get(const Point2d & pos, const size_t sz, HeightMap & hm) const
{
	if (pos.x() >= totalSize_ || pos.y() >= totalSize_)
	{
		throw std::runtime_error("Out of constant loader range");
	}

	const float value = sz == 1 ? pointValue_ : patchValue_;

	const float increment = (pos.y() * totalSize_ + pos.x()) * increment_;

	HeightMap::Container v(sz * sz, value + increment);
	hm.Swap(v);
}

void 
HeightMapLoaderConstant::Set(const Point2d & /*pos*/, const HeightMap & /*hm*/)
{
	throw std::logic_error("Not implemented");
}