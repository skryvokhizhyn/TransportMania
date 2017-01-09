#include "PatchGrid.h"
#include "ModelData.h"
#include "Point3d.h"
#include "Point2d.h"
#include "WorldProjection.h"
#include "GeometryUtils.h"
#include "Logger.h"
#include "GlobalDefines.h"
#include "PolygonVisibilityChecker.h"
#include "TextureManagerProxy.h"

#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/find_if.hpp>

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
		Point3d center = wp.GetShiftPosition();
		center.z() = 0.0f;

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

	float GetCornerHeight(const terrain::HeightMapLoader & hml, const Point2d & cornerPoint)
	{
		const Point2d defaultGetterPoint(0, 0);
		HeightMap hm;

		hml.Get(cornerPoint, 1, hm);

		return hm.At(defaultGetterPoint);
	}

	Point3d GetCornerPoint(const CornerValues & heights, const Point2d & point, const int id)
	{
		Point3d p = Point3d::Cast(point);
		p.z() = heights.at(id);

		return p;
	}

	bool CheckPatchVisible(const CornerValues & heights, const Size2d & s, 
		const Matrix & pv, size_t patchSize)
	{
		/*if (s.x() > 20 || s.y() > 20)
			return false;

		(s); (wp); (patchSize); (heights);
		return true;*/

		--patchSize;
		const float psf = boost::numeric_cast<float>(patchSize);
		const Point2d s2d = Point2d::Cast(s);
		
		// left bottom
		const Point3d lb = GetCornerPoint(heights, s2d, PatchCorner::LeftBottom);
		// left up
		const Point3d lu = GetCornerPoint(heights, s2d + Point2d(0, psf), PatchCorner::LeftUp);
		// right up
		const Point3d ru = GetCornerPoint(heights, s2d + Point2d(psf, psf), PatchCorner::RightUp);
		// right bottom
		const Point3d rb = GetCornerPoint(heights, s2d + Point2d(psf, 0), PatchCorner::RightBottom);

		return CheckPolygonIsVisible(pv, {lb, lu, ru, rb});
	}

	CornerValues GetPatchCornerHeights(const terrain::HeightMapLoader & hml, const Size2d & s, size_t patchSize)
	{
		CornerValues vals(4, 0.0f);

		--patchSize;
		const float psf = boost::numeric_cast<float>(patchSize);
		const Point2d s2d = Point2d::Cast(s);
		
		// left bottom
		vals[PatchCorner::LeftBottom] = GetCornerHeight(hml, s2d);
		// left up
		vals[PatchCorner::LeftUp] = GetCornerHeight(hml, s2d + Point2d(0, psf));
		// right up
		vals[PatchCorner::RightUp] = GetCornerHeight(hml, s2d + Point2d(psf, psf));
		// right bottom
		vals[PatchCorner::RightBottom] = GetCornerHeight(hml, s2d + Point2d(psf, 0));

		return vals;
	}

	template<typename Callback>
	void DoForAllValid(const PatchGridMap & pgm, const Callback & cb)
	{
		std::find_if(pgm.Begin(), pgm.End(),
			[&](const PatchGridNode & node)
		{
			if (!node.data.valid)
				return true;

			cb(node);

			return false;
		});
	}

	void RenderNode(const PatchGridNode & node, PointNormaleMap & globalNormales)
	{
		assert(node.data.valid);

		const Size2d & pos = node.pos;
		Point3d shift = Point3d::Cast(pos);

		Patch & p = node.data.patch;
		ModelData & md = node.data.renderCache;
		md.Clear();

		PointNormaleMap localNormales;

		p.Render(md, localNormales);

		boost::transform(md.points, md.points.begin(), boost::bind(std::plus<Point3d>(), shift, _1));
		boost::for_each(localNormales.Data(), 
			[&](const PointNormaleMap::NormaleMap::value_type & value)
		{
			globalNormales.Put(value.first + pos, value.second);
		});
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
PatchGrid::Update(const Matrix & projectionViewMatrix)
{
	auto it = grid_.Begin();
	auto end = grid_.End();

	while (it != end)
	{
		const Size2d & pos = it->pos;
		auto & data = it->data;

		HeightMap & hm = data.heightMap;
		const bool isValid = data.valid;
		CornerValues & cornerValues = data.heights;
		
		const Point3d t = Point3d::Cast(pos);

		//const bool isVisible = IsVisible(wp, t, patchSize_);

		if (cornerValues.empty())
		{
			cornerValues = GetPatchCornerHeights(*hmlPtr_, pos, patchSize_);
		}
		const bool isVisible = CheckPatchVisible(cornerValues, pos, projectionViewMatrix, patchSize_);

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
			data.dirty = false;
			data.valid = false;
		}
	}
}

bool 
PatchGrid::Tasselate(const Point3d & camera)
{
	bool wasUpdated = false;

	DoForAllValid(grid_, 
		[&wasUpdated, &camera](const PatchGridNode & node)
	{
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

void 
PatchGrid::Render()
{
	DoForAllValid(grid_, boost::bind(&RenderNode, _1, boost::ref(normaleMap_)));

#ifdef DRAWING_MODE_FULL

	DoForAllValid(grid_, 
		[this](const PatchGridNode & node)
	{
		ModelData & md = node.data.renderCache;
		md.normales.reserve(md.points.size());

		boost::range::for_each(md.points,
			[&](const Point3d & p)
		{
			const Size2d s = Size2d::Cast(p);
			md.normales.push_back(normaleMap_.At(s));
		});
	});

#endif // DRAWING_MODE_FULL

	normaleMap_.Clear();
}

namespace
{
	void PrepareTerrainTextureData(const Size2d & pos, ModelData & md)
	{
		const Point2d & posP = Point2d::Cast(pos);

		md.textureId = TextureId::Ground;
		md.textures.reserve(md.points.size());
		boost::transform(md.points, std::back_inserter(md.textures),
			[&](const Point3d & p) -> Point2d
		{
			Point2d t = Point2d::Cast(p) / 16.0f;

			float intPart = 0.0f;
			t.x() = std::modf(t.x(), &intPart);
			t.y() = std::modf(t.y(), &intPart);

			if (utils::CheckEqual(t.x(), 0.0f) && !utils::CheckEqual(posP.x(), p.x()))
			{
				t.x() = 1.0f;
			}

			if (utils::CheckEqual(t.y(), 0.0f) && !utils::CheckEqual(posP.y(), p.y()))
			{
				t.y() = 1.0f;
			}

			return t;
		});
	}
}

bool 
PatchGrid::GetNextRenderResult(ModelData & md)
{
	if (currIt_ == grid_.End() || !currIt_->data.valid)
		return false;

	md = std::move(currIt_->data.renderCache);

	PrepareTerrainTextureData(currIt_->pos, md);

	++currIt_;

	return true;
}

size_t 
PatchGrid::GetSize() const
{
	// amount of points in patch * amount of patches + 1 as we have 2^n + 1 points
	return (patchSize_ - 1) * patchCount_ + 1;
}

HeightMap::Value 
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
PatchGrid::Set(const Point2d & p, const HeightMap::Value z)
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

			// remove cached values so they are re-taken from the height map
			found->data.heights.clear();
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
