#include "TerrainCoverDrawableObject.h"
#include "ContainerAggregator.h"
#include "Triangle3d.h"
#include "GeometryUtils.h"

#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/irange.hpp>

#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/for_each.hpp>

#pragma warning(push)
#pragma warning(disable: 4512)

#include <boost/assign/std/vector.hpp>

#pragma warning(pop)

using namespace trm;

namespace
{
	const float COVER_Z_SHIFT = 0.5f;

	void ProcessPointBlock(const Point2d & p, 
		const TerrainCoverDrawableObject::PointMap::left_map & pm, IndexVector & iv)
	{
		boost::optional<int> ld, lu, rd, ru;
		using namespace boost::assign;

		TerrainCoverDrawableObject::PointMap::left_map::const_iterator it;
		TerrainCoverDrawableObject::PointMap::left_map::const_iterator itEnd = pm.end();

		if ((it = pm.find(p + Point2d(-1.0f, 0.0f))) != itEnd)
		{
			ld = it->second;
		}
		if ((it = pm.find(p)) != itEnd)
		{
			rd = it->second;
		}
		if ((it = pm.find(p + Point2d(-1.0f, 1.0f))) != itEnd)
		{
			lu = it->second;
		}
		if ((it = pm.find(p + Point2d(0.0f, 1.0f))) != itEnd)
		{
			ru = it->second;
		}

		if (!ld && rd && lu && ru)
		{
			iv += lu.get(), rd.get(), ru.get();
		}
		else if (ld && rd && lu && ru)
		{
			iv += ld.get(), ru.get(), lu.get();
			iv += ld.get(), rd.get(), ru.get();
		}
		else if (ld && rd && !lu && ru)
		{
			iv += ld.get(), rd.get(), ru.get();
		}
		else if (ld && !rd && lu && ru)
		{
			iv += ld.get(), ru.get(), lu.get();
		}
		else if (ld && rd && lu && !ru)
		{
			iv += lu.get(), ld.get(), rd.get();
		}
	}
}

TerrainCoverDrawableObject::TerrainCoverDrawableObject(const PointVector & points)
	: points_(points)
{}

void 
TerrainCoverDrawableObject::Render(ModelData & md)
{
	md.points.reserve(points_.size());
	md.points = points_;
	boost::transform(md.points, md.points.begin(), 
		[](const Point3d & p)
	{
		return p + Point3d(0.0f, 0.0f, COVER_Z_SHIFT);
	});

	PointMap ids;
	boost::transform(md.points | boost::adaptors::indexed(0), std::inserter(ids, ids.begin()),
		[](const boost::indexed_range<PointVector>::const_iterator::value_type & val)
	{
		return PointMap::value_type(Point2d::Cast(val.value()), val.index());
	});

	md.indexes = GetTriangulationIndexes(ids);

	NormaleMap normaleMap = GetNormales(md.points, md.indexes);
	md.normales.reserve(points_.size());
	boost::copy(normaleMap | boost::adaptors::map_values, std::back_inserter(md.normales));

	md.textures.reserve(md.points.size());
	boost::transform(md.points, std::back_inserter(md.textures),
		[](const Point3d & p)
	{
		return Point2d::Cast(p);
	});

	md.textureId = TextureId::TransparentGray;
	md.type = ModelData::Mode::Triangle;
}

IndexVector 
TerrainCoverDrawableObject::GetTriangulationIndexes(const PointMap & pm)
{
	using PointBoundary = std::map<int, std::pair<int, int>>;
	PointBoundary boundary;
	
	boost::for_each(pm.left | boost::adaptors::map_keys,
		[&](const Point2d & p)
	{
		const int x = static_cast<int>(p.x());
		const int y = static_cast<int>(p.y());
		
		auto found = boundary.find(static_cast<int>(y));
		if (found == boundary.end())
		{
			boundary.emplace(y, std::make_pair(x, x));
		}
		else
		{
			found->second.first = std::min(x, found->second.first);
			found->second.second = std::max(x, found->second.second);
		}
	});

	IndexVector indx;

	boost::for_each(boundary, 
		[&](const PointBoundary::value_type & vt)
	{
		const int y = vt.first;
		const int xMin = vt.second.first;
		const int xMax = vt.second.second + 1;

		boost::for_each(boost::irange(xMin, xMax + 1), 
			[&](const int xCurr)
		{
			ProcessPointBlock(
				Point2d(
					static_cast<float>(xCurr), 
					static_cast<float>(y)), 
				pm.left, indx);
		});
	});

	return indx;
}

TerrainCoverDrawableObject::NormaleMap 
TerrainCoverDrawableObject::GetNormales(const PointVector & pv, const IndexVector & iv)
{
	NormaleMap normales;

	auto normaleCalculator = [&](int a, int b, int c)
	{
		const Point3d normale = utils::GetNormaleForTriangleNonNormalized(pv.at(a), pv.at(b), pv.at(c));

		normales[a] += normale;
		normales[b] += normale;
		normales[c] += normale;
	};

	utils::ContainerAggregator<decltype(normaleCalculator), int, 3> aggregator(normaleCalculator);

	boost::for_each(iv, aggregator);

	return normales;
}
