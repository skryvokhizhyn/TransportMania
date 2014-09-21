#include "PatchGrid.h"
#include "ModelData.h"
#include "Point3d.h"
#include "Point2d.h"
#include "WorldProjection.h"
#include "GeometryUtils.h"
#include "Logger.h"
#include "GlobalDefines.h"

#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/combine.hpp>

#include <cstdlib>

using namespace trm;
using namespace trm::terrain;
using namespace trm::terrain::lod;

namespace
{
	Size2d GetNearestPatchPosition(const Point2d & camera, const unsigned short pSz)
	{
		Point2d pos = camera / pSz;
		
		return Size2d::Cast(pos) * pSz;
	}

	bool IsVisible(const WorldProjection & wp, Point3d p, const size_t width)
	{
		const Point3d center = wp.GetScreenCenter();

		p += width / 2.0f;
		p.z() = 0;

		const float radii = width / ::sqrtf(2);
		const float dist = utils::GetDistance(center, p);

		if (radii < dist)
		{
			const float ratio = radii / dist;
		
			const Point3d border = p - (p - center) * ratio;

			return wp.IsPointVisible(border);
		}
		else
		{
			return true;
		}
	}
}

PatchGrid::PatchGrid(const unsigned short patchSize, const unsigned short patchCnt, const HeightMapLoaderPtr & hmlPtr)
	: patchSize_(patchSize), patchCount_(patchCnt), hmlPtr_(hmlPtr)
{
	if (patchSize_ == 0)
	{
		throw std::runtime_error("Zero patch Size given");
	}
}

PatchGrid::PatchGrid(PatchGrid && pg)
	: patchSize_(pg.patchSize_)
	, patchCount_(pg.patchCount_)
	, hmlPtr_(std::move(pg.hmlPtr_))
	, grid_(std::move(pg.grid_))
{
}

void
PatchGrid::Init()
{
	const unsigned short patchWidth = patchSize_ - 1;
	const size_t det = utils::GetPowerOf2(patchWidth);

	for (unsigned short x = 0; x < patchCount_; ++x)
	{
		for (unsigned short y = 0; y < patchCount_; ++y)
		{
			const Size2d pos = Size2d(x, y) * patchWidth;
			auto it = grid_.Init(pos);
			HeightMap & hm = it->data.heightMap;
			Patch & p = it->data.patch;
			
			p.Init(hm, det * 2);
			
			if (y > 0)
			{
				auto found = grid_.Find(Size2d(x, (y - 1)) * patchWidth);
				found->data.patch.Attach(p, Direction::Up);
			}

			if (x > 0)
			{
				auto found = grid_.Find(Size2d((x - 1), y) * patchWidth);
				found->data.patch.Attach(p, Direction::Right);
			}
		}
	}
}

void 
PatchGrid::LoadHeightMap(const Size2d & pos, HeightMap & hm) const
{
	if (!hm.IsLoaded())
	{
		hmlPtr_->Get(Point2d::Cast(pos), patchSize_, hm);
	}
}

void
PatchGrid::Update(const WorldProjection & wp)
{
	auto it = grid_.Begin();
	auto end = grid_.End();

	while (it != end)
	{
		const Size2d & pos = it->pos;
		HeightMap & hm = it->data.heightMap;

		const Point3d t = Point3d::Cast(pos);

		auto & data = it->data;
		const bool isValid = data.valid;
		const bool isVisible = IsVisible(wp, t, patchSize_);
		/*bool isVisible = false;
		if (pos == Size2d(32, 0))
			isVisible = true;
		(wp);*/

		// visible means that should be valid
		// if visible and not valid or visa-versa then...
		if (data.valid != isVisible)
		{
			auto itCurrent = it++;

			if (isVisible)
			{
				grid_.MoveToBegin(itCurrent);
			}
			else
			{
				grid_.MoveToEnd(itCurrent);
			}
		}
		else
		{
			++it;
		}

		if (!isValid && isVisible)
		{
			LoadHeightMap(pos, hm);
			data.dirty = true;
			data.valid = true;
		}
		else if (isValid && !isVisible)
		{
			hm.Clear();
			data.patch.Clear();
			data.valid = false;
		}
	}
}

bool 
PatchGrid::Tasselate(const WorldProjection & wp)
{
	bool wasUpdated = false;

	const Point3d & camera = wp.GetCameraPosition();

	std::find_if(grid_.Begin(), grid_.End(),
		[&](const PatchGridNode & node)
	{
		if (!node.data.valid)
		{
			return true;
		}

		Patch & p = node.data.patch;

		// Before tasselation and other stuff
		if (node.data.dirty)
		{
			p.ComputeVariance();
			node.data.dirty = false;
		}

		const Size2d & pos = node.pos;
		Point3d shift = Point3d::Cast(pos);

		// if any was updated
		wasUpdated |= p.Tasselate(camera - shift);

		return false;
	});

	currIt_ = grid_.Begin();

	return wasUpdated;
}

bool 
PatchGrid::Render(ModelData & md)
{
	if (currIt_ == grid_.End() || !currIt_->data.valid)
		return false;

	RenderNode(*currIt_, md);

#ifdef DRAWING_MODE_FULL
	//GlueNormales(*currIt_, md);
#endif // DRAWING_MODE_FULL

	++currIt_;

	return true;
}

void 
PatchGrid::GlueNormales(const PatchGridNode & node, ModelData & md) const
{
	const Size2d & patchPos = node.pos;

	const auto r = boost::combine(md.points, md.normales);

	for (auto rr : r)
	{
		const Point3d & p = rr.get<0>();

		const Point2d p2d = Point2d::Cast(p);
		const Size2d & sizePoint = Size2d::Cast(p2d);

		if (!((sizePoint.x() % (patchSize_ - 1) == 0)
			&& (sizePoint.y() % (patchSize_ - 1) == 0)))
			continue;

		const Positions pos = GetAdjucentPatches(p2d);

		boost::range::for_each(pos,
			[&](const Size2d & s)
		{
			if (s != patchPos)
			{
				Point3d & n = rr.get<1>();

				const auto found = grid_.Find(s);
				
				const Patch & patch = found->data.patch;
				const PointNormaleMap & normales = patch.GetNormales();

				const Size2d shiftedPoint = sizePoint - s;

				n += normales.At(shiftedPoint);

				/*const auto foundNormale = normales.find(shiftedPoint);
				if (foundNormale != normales.end())
				{
					n += foundNormale->second;
				}*/
			}
		});
	}
}

void 
PatchGrid::RenderNode(const PatchGridNode & node, ModelData & md)
{
	if (!node.data.valid)
	{
		return;
	}

	const Size2d & pos = node.pos;
	Point3d shift = Point3d::Cast(pos);

	const size_t sz = md.points.size();

	Patch & p = node.data.patch;

	p.Render(md);
	p.ZipNormales();

	PointVector::iterator start = md.points.begin();
	std::advance(start, sz);

	std::transform(start, md.points.end(), start, boost::bind(std::plus<Point3d>(), shift, _1));
}

size_t 
PatchGrid::GetSize() const
{
	// amount of points in patch * amount of patches + 1 as we have 2^n + 1 points
	return (patchSize_ - 1) * patchCount_ + 1;
}

HeightMap::Type 
PatchGrid::At(const Point2d & p) const
{
	const Positions pos = GetAdjucentPatches(p);

	if (pos.empty())
	{
		std::stringstream ss;
		ss << "No patch found for position " << p;
		throw std::runtime_error(ss.str());
	}

	const Size2d & patchPos = pos.front();

	const auto found = grid_.Find(patchPos);
	
	HeightMap & hm = found->data.heightMap;
	LoadHeightMap(found->pos, hm);

	return hm.At(p - Point2d::Cast(patchPos));
}

void 
PatchGrid::Set(const Point2d & p, const HeightMap::Type z)
{
	const Positions pos = GetAdjucentPatches(p);

	for_each(pos.begin(), pos.end(),
		[&](const Size2d & s)
		{
			const auto found = grid_.Find(s);
			
			HeightMap & hm = found->data.heightMap;
			LoadHeightMap(found->pos, hm);

			hm.Set(p - Point2d::Cast(s), z);

			found->data.dirty = true;
		});
}

void 
PatchGrid::Flush()
{
	std::for_each(grid_.Begin(), grid_.End(),
	[&](const PatchGridNode & node)
	{
		if (node.data.dirty)
		{
			hmlPtr_->Set(Point2d::Cast(node.pos), node.data.heightMap);
		}
	});
}

auto PatchGrid::GetAdjucentPatches(const Point2d & p) const -> Positions
{
	Positions pos;

	const Size2d absPos = Size2d::Cast(p);
	const Size2d patchPos = GetNearestPatchPosition(p, patchSize_ - 1);

	const size_t gridSize = GetSize();

	typedef std::vector<size_t> PositionVec;
	PositionVec posX;
	posX.reserve(2);
	PositionVec posY;
	posY.reserve(2);

	if (absPos.x() == patchPos.x() && absPos.x() > 0)
	{
		posX.push_back(patchPos.x() - patchSize_ + 1);
	}
		
	if (absPos.x() < gridSize - 1)
	{
		posX.push_back(patchPos.x());
	}

	if (absPos.y() == patchPos.y() && absPos.y() > 0)
	{
		posY.push_back(patchPos.y() - patchSize_ + 1);
	}

	if (absPos.y() < gridSize - 1)
	{
		posY.push_back(patchPos.y());
	}

	Positions result;
	result.reserve(4);

	for (PositionVec::const_iterator itX = posX.begin(), endX = posX.end(); itX != endX; ++itX)
	{
		for (PositionVec::const_iterator itY = posY.begin(), endY = posY.end(); itY != endY; ++itY)
		{
			result.push_back(Size2d(*itX, *itY));
		}
	}

	return result;
}
