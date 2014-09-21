#include "Terraformer.h"
#include "TerrainRange.h"
#include "Size2d.h"
#include "Point2d.h"
#include "HeightMap.h"
#include "Terrain.h"
#include <boost/foreach.hpp>

using namespace trm;
using namespace trm::terrain;

bool 
Terraformer::Normalized(const Size2d & sz, TerrainRange::Range & r)
{
	const int x = static_cast<int>(sz.x());
	const int y = static_cast<int>(sz.y());

	if (r.y >= y)
	{
		return false;
	}
	if (r.xBegin >= x)
	{
		return false;
	}
	if (r.xEnd < 0)
	{
		return false;
	}

	if (r.y < 0)
	{
		r.y = 0;
	}

	if (r.xBegin < 0)
	{
		r.xBegin = 0;
	}

	if (r.xEnd < 0)
	{
		r.xEnd = 0;
	}

	if (r.y >= y)
	{
		r.y = y - 1;
	}

	if (r.xBegin >= x)
	{
		r.xBegin = x - 1;
	}

	if (r.xEnd >= x)
	{
		r.xEnd = x - 1;
	}

	return true;
}

Terraformer::Terraformer(const TerrainRange & range, TerraformFunction func)
	: range_(range), func_(func)
{}

void 
Terraformer::Apply(HeightMapBase & hm)
{
	const size_t sz = hm.GetSize();

	TerrainRange::Ranges ranges = range_.GetRanges();

	const Size2d size(sz, sz);

	BOOST_FOREACH(TerrainRange::Range r, ranges)
	{
		if (!Normalized(size, r))
			continue;
		
		while (r.xBegin <= r.xEnd)
		{
			const Point2d at(static_cast<float>(r.xBegin), static_cast<float>(r.y));

			const HeightMap::Type zOld = hm.At(at);
			HeightMap::Type zNew = func_(at, zOld);

			if (zNew > Terrain::MAX_HEIGHT)
			{
				zNew = Terrain::MAX_HEIGHT;
			}
			else
			if (zNew < Terrain::MIN_HEIGHT)
			{
				zNew = Terrain::MIN_HEIGHT;
			}

			hm.Set(at, zNew);
			++r.xBegin;
		}
	}
}

//const TerrainRange & 
//Terraformer::GetRange() const
//{
//	return range_;
//}