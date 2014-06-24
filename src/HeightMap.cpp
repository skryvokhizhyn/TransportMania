#include "HeightMap.h"
#include "Point2d.h"
#include <boost/format.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cmath>

using namespace trm;
using namespace trm::terrain;

HeightMap::HeightMap()
: sz_(0)
{}

HeightMap::~HeightMap()
{}

HeightMap::Type 
HeightMap::At(const Point2d & pos) const
{
	return height_.at(boost::numeric_cast<size_t>(pos.y() * sz_ + pos.x()));
}

size_t
HeightMap::GetSize() const
{
	return sz_;
}

size_t
HeightMap::GetSideSize(const size_t len)
{
	const size_t sz = static_cast<size_t>(::sqrt(static_cast<float>(len)));
	if (sz * sz != len)
	{
		throw std::runtime_error((boost::format("Enormous size of terrain. Must be n^2 like but given %d") % len).str());
	}

	return sz;
}

void 
HeightMap::Set(const Point2d & pos, const Type z)
{
	height_.at(boost::numeric_cast<size_t>(pos.y() * sz_ + pos.x())) = z;
}

void 
HeightMap::Clear()
{
	HeightMapType().swap(height_);
}

void
HeightMap::Swap(HeightMapType & hm)
{
	height_.swap(hm);
	sz_ = GetSideSize(height_.size());
}

auto HeightMap::GetData() const -> const HeightMapType & 
{
	return height_;
}

bool 
HeightMap::IsLoaded() const
{
	return !height_.empty();
}