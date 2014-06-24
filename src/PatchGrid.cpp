#include "PatchGrid.h"
#include "ModelData.h"
#include "HeightMapLoader.h"
#include "Point3d.h"
#include "Point2d.h"
#include "Size2d.h"
#include "WorldProjection.h"
#include "GeometryUtils.h"
#include "Logger.h"
#include "GlobalDefines.h"
#include "TerrainRange.h"

using namespace trm;
using namespace trm::terrain;
using namespace trm::terrain::lod;

namespace
{
	enum
	{
		//PositionTag = 0,
		HeightMapTag = 0,
		PatchTag = 1
	};

	Size2d GetNearestPatchPosition(const Point2d & camera, const unsigned short pSz)
	{
		Point2d pos = camera / pSz;

		//pos.x() = ::floor(camera.x() / PatchGrid::pSz);
		//pos.y() = ::floor(camera.y() / PatchGrid::pSz);

		//pos.x() = ::floor(pos.x());
		//pos.y() = ::floor(pos.y());
		
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

	for (unsigned short x = 0; x < patchCount_; ++x)
	//for (unsigned short x = 7; x < 8; ++x)
	{
		for (unsigned short y = 0; y < patchCount_; ++y)
		//for (unsigned short y = 3; y < 5; ++y)
		{
			const Size2d rp = Size2d(x, y) * patchWidth;
			NodePtr rn = CreateNode(/*rp * PATCH_SIZE, hml*/patchWidth);
			PutNode(rp, rn);
			
			if (y > 0)
			//if (y > 3)
			{
				//auto found = grid_.find(Point2d(x, (float)(y - 1)) * patchWidth);
				auto found = grid_.find(Size2d(x, (y - 1)) * patchWidth);
				found->second->get<PatchTag>().Attach(rn->get<PatchTag>(), Direction::Up);
			}

			if (x > 0)
			//if (x > 7)
			{
				//auto found = grid_.find(Point2d((float)(x - 1), y) * patchWidth);
				auto found = grid_.find(Size2d((x - 1), y) * patchWidth);
				found->second->get<PatchTag>().Attach(rn->get<PatchTag>(), Direction::Right);
			}
		}
	}
}

auto PatchGrid::CreateNode(/*const Point2d & pos*/const unsigned short pSz) -> NodePtr
{
	PatchGrid::NodePtr node(new PatchGrid::Node());
	//node->get<PositionTag>() = pos;

	HeightMap & hm = node->get<HeightMapTag>();
	/*hml.GetHeightMap(pos, PATCH_SIZE + 1, hm);*/

	const size_t det = utils::GetPowerOf2(pSz);
	node->get<PatchTag>().Init(hm, det * 2);

	return node;
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
	for (auto it = grid_.begin(); it != grid_.end(); ++it)
	{
		//const Point2d & pos = it->first;
		const Size2d & pos = it->first;
		Patch & patch = it->second->get<PatchTag>();
		HeightMap & hm = it->second->get<HeightMapTag>();

		const Point3d t = Point3d::Cast(pos);

		const bool isValid = patch.GetValid();
		const bool isVisible = IsVisible(wp, t, patchSize_);

		if (!isValid && isVisible)
		{
			//hml_.Get(Point2d::Cast(pos), patchSize_, hm);
			LoadHeightMap(pos, hm);
			patch.SetDirty();
			patch.SetValid();

			//utils::Logger().Debug() << pos << " Updated";
		}
		else if (isValid && !isVisible)
		{
			hm.Clear();
			patch.Clear();

			//utils::Logger().Debug() << pos << " Became invisible";
		}
	}

	currIt_ = grid_.begin();
}

#include <boost/range/algorithm/for_each.hpp>

void 
PatchGrid::Tasselate(const WorldProjection & wp)
{
	const Point3d & camera = wp.GetCameraPosition();

	boost::range::for_each(grid_,
		[&](const GridMapType::value_type & node)
	{
		Patch & p = node.second->get<PatchTag>();
		
		if (!p.GetValid())
		{
			return;
		}

		// Before tasselation and other stuff
		if (p.GetDirty())
		{
			p.ComputeVariance();
		}

		const Size2d & pos = node.first;
		Point3d shift = Point3d::Cast(pos);

		p.Tasselate(camera - shift);
	});
}

bool 
PatchGrid::Render(ModelData & md)
{
	if (currIt_ == grid_.end())
		return false;

	RenderNode(*currIt_, md);

#ifdef DRAWING_MODE_FULL
	GlueNormales(*currIt_, md);
#endif // DRAWING_MODE_FULL

	++currIt_;

	return true;
}

#include <boost/range/combine.hpp>
#include <boost/range/algorithm/for_each.hpp>

void 
PatchGrid::GlueNormales(const GridMapType::value_type & node, ModelData & md) const
{
	const Size2d & patchPos = node.first;

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

				const auto found = grid_.find(s);
				if (found == grid_.end())
				{
					throw std::runtime_error("Patch for normales is missing");
				}

				const Patch & patch = found->second->get<PatchTag>();
				const PointNormaleMap & normales = patch.GetNormales();

				const Size2d shiftedPoint = sizePoint - s;
				//const Size2d shiftedPoint = Size2d::Cast(p2d) - s;

				const auto foundNormale = normales.find(shiftedPoint);
				if (foundNormale != normales.end())
				{
					n += foundNormale->second;
				}
			}
		});
	}
}

void 
PatchGrid::RenderNode(const GridMapType::value_type & node, ModelData & md)
{
	Patch & p = node.second->get<PatchTag>();
		
	if (!p.GetValid())
	{
		return;
	}

	const Size2d & pos = node.first;
	Point3d shift = Point3d::Cast(pos);

	const size_t sz = md.points.size();

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

#include <cstdlib>
#include <boost/numeric/conversion/cast.hpp>

HeightMap::Type 
PatchGrid::At(const Point2d & p) const
{
	//Size2d pos = GetNearestPatchPosition(p, patchSize_ - 1);

	const Positions pos = GetAdjucentPatches(p);

	if (pos.empty())
	{
		std::stringstream ss;
		ss << "No patch found for position " << p;
		throw std::runtime_error(ss.str());
	}

	const Size2d & patchPos = pos.front();

	const auto found = grid_.find(patchPos);
	if (found == grid_.end())
	{
		throw std::runtime_error("Point is out of grid");
	}

	HeightMap & hm = found->second->get<HeightMapTag>();
	LoadHeightMap(found->first, hm);

	return hm.At(p - Point2d::Cast(patchPos));
}

void 
PatchGrid::Set(const Point2d & p, const HeightMap::Type z)
{
	const Positions pos = GetAdjucentPatches(p);

	for_each(pos.begin(), pos.end(),
		[&](const Size2d & s)
		{
			const auto found = grid_.find(s);
			if (found == grid_.end())
			{
				throw std::runtime_error("Point is out of grid");
			}
	
			//found->second->get<HeightMapTag>().Set(p - Point2d::Cast(s), z);

			HeightMap & hm = found->second->get<HeightMapTag>();
			LoadHeightMap(found->first, hm);

			hm.Set(p - Point2d::Cast(s), z);

			found->second->get<PatchTag>().SetDirty();
		});
}

void 
PatchGrid::PutNode(const Size2d & pos, NodePtr nodePtr)
{
	grid_.insert(std::make_pair(pos, nodePtr));
}

void 
PatchGrid::Flush()
{
	for(const GridMapType::value_type & val : grid_)
	{
		if (val.second->get<PatchTag>().GetDirty())
		{
			hmlPtr_->Set(Point2d::Cast(val.first), val.second->get<HeightMapTag>());
		}
	}
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
