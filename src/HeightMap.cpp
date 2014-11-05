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

//HeightMap::HeightMap(const HeightMap & h)
//	: sz_(h.sz_)
//	, height_(height_)
//{
//}
		
HeightMap::HeightMap(HeightMap && h)
	: sz_(sz_)
	, height_(std::move(h.height_))
{
	h.sz_ = 0;
}

HeightMap::~HeightMap()
{}

HeightMap::Value 
HeightMap::At(const Point2d & pos) const
{
	//return height_.at(boost::numeric_cast<size_t>(pos.y() * sz_ + pos.x()));
	return GetValueAt(pos, height_, sz_);
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

HeightMap::Value 
HeightMap::GetValueAt(const Point2d & p, const Container & c, const size_t s)
{
	return c.at(boost::numeric_cast<size_t>(p.y() * s + p.x()));
}

void 
HeightMap::Set(const Point2d & pos, const Type z)
{
	height_.at(boost::numeric_cast<size_t>(pos.y() * sz_ + pos.x())) = z;
}

void 
HeightMap::Clear()
{
	Container().swap(height_);
}

void
HeightMap::Swap(Container & hm)
{
	sz_ = GetSideSize(hm.size());
	height_.swap(hm);
}

auto HeightMap::GetData() const -> const Container & 
{
	return height_;
}

bool 
HeightMap::IsLoaded() const
{
	return !height_.empty();
}

HeightMap::Container 
trm::terrain::PrepareDataFromVectorReversed(const HeightMap::Value * arr, const size_t n)
{
	const size_t sz = HeightMap::GetSideSize(n);

	HeightMap::Container height(n);

	size_t i = 0;
	size_t j = sz;

	while (j > 0)
	{
		height[(j - 1) * sz + i] = arr[(sz - j) * sz + i];

		if (++i == sz)
		{
			--j;
			i = 0;
		}
	}

	return height;
}