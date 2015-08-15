#include "ItemLocator.h"
#include "Point2d.h"

// 4503 decorated name length exceeded, name was truncated
// 4512 assignment operator could not be generated
#pragma warning(disable: 4512 4503)

#include <boost/geometry/index/rtree.hpp>

#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include <boost/geometry/algorithms/envelope.hpp>
#include <boost/geometry/algorithms/correct.hpp>
#include <boost/geometry/algorithms/make.hpp>

#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/for_each.hpp>

using namespace trm;

namespace bg = boost::geometry;

using BGPoint2d = bg::model::d2::point_xy<double>;
using Box2d = bg::model::box<BGPoint2d>;
using BGPolygon2d = bg::model::polygon<BGPoint2d>;

namespace priv
{
	template<typename Container>
	class MapIndexableGetter
	{
	public:
		using ConstRef = typename Container::mapped_type;
		using ConstContainerRef = std::reference_wrapper<const Container>;
		using result_type = ConstRef;

	public:
		explicit MapIndexableGetter(const Container & c)
			: cRef_(c)
		{}
	
		ConstRef operator() (UniqueId i) const 
		{
			return cRef_.get().at(i);
		}

	private:
		ConstContainerRef cRef_;
	};

} // namespace priv

namespace
{
	BGPolygon2d ConvertToBGPolygon2d(const Polygon2d & p)
	{
		BGPolygon2d bgPolygon;
		bgPolygon.outer().reserve(p.size() + 1);

		boost::transform(p, std::back_inserter(bgPolygon.outer()), 
			[](const Point2d & p2d)
		{
			return bg::make<BGPoint2d>(p2d.x(), p2d.y());
		});

		bg::correct(bgPolygon);

		return bgPolygon;
	}

	Box2d ConvertToBox2d(const Polygon2d & polygon)
	{
		BGPolygon2d polygon2d = ConvertToBGPolygon2d(polygon);
		
		return bg::return_envelope<Box2d>(polygon2d);
	}
}

class ItemLocator::ItemLocatorImpl
{
	using IdToBoxMap = std::unordered_map<UniqueId, Box2d>;
	using RTreeValue = IdToBoxMap::key_type;
	using MapIndexableType = priv::MapIndexableGetter<IdToBoxMap>;
	using CreationAlgo = bg::index::rstar<16u>;
	using RTree = bg::index::rtree<RTreeValue, CreationAlgo, MapIndexableType>;

public:
	ItemLocatorImpl()
		: indexable_(idToBoxMap_)
		, rTree_(CreationAlgo(), indexable_)
	{}

	void Put(UniqueId id, const Polygon2d & polygon)
	{
		const Box2d box = ConvertToBox2d(polygon);

		if (!idToBoxMap_.emplace(id, box).second)
		{
			throw std::runtime_error(boost::str(boost::format("Item with id=%d is already in the locator") % id));
		}

		rTree_.insert(id);
	}

	void Remove(UniqueId id)
	{
		const size_t rTreeRemovedCnt = rTree_.remove(id);
		const size_t idToBoxMapRemovedCnt = idToBoxMap_.erase(id);

		if (rTreeRemovedCnt != idToBoxMapRemovedCnt || idToBoxMapRemovedCnt == 0)
		{
			throw std::runtime_error(boost::str(boost::format("Trying to remove unknow item id=%d from locator") % id));
		}
	}

	Ids At(const Polygon2d & polygon) const
	{
		const Box2d box = ConvertToBox2d(polygon);
		Ids ids;

		rTree_.query(bg::index::intersects(box), std::back_inserter(ids));

		return ids;
	}

private:
	IdToBoxMap idToBoxMap_;
	MapIndexableType indexable_;
	RTree rTree_;
};

ItemLocator::ItemLocator()
	: implPtr_(new ItemLocatorImpl())
{}

ItemLocator::~ItemLocator()
{}

void 
ItemLocator::Put(UniqueId id, const Polygon2d & p)
{
	Polygon2d polygonCopy = p;
	Put(id, std::move(polygonCopy));
}

void 
ItemLocator::Put(UniqueId id, Polygon2d && p)
{
	implPtr_->Put(id, p);
	if (!polyMap_.emplace(id, std::move(p)).second)
	{
		throw std::runtime_error(boost::str(boost::format("Polygon with id=%d is already in the locator") % id));
	}
}

void 
ItemLocator::Remove(UniqueId id)
{
	implPtr_->Remove(id);
	if (polyMap_.erase(id) == 0)
	{
		throw std::runtime_error(boost::str(boost::format("Trying to remove unknow polygon id=%d from locator") % id));
	}
}

ItemLocator::Ids 
ItemLocator::At(const Polygon2d & p) const
{
	Ids resultIds;

	const BGPolygon2d requestedPolygon = ConvertToBGPolygon2d(p);

	boost::for_each(implPtr_->At(p),
		[&](const UniqueId & id)
	{
		const auto foundPolygond = polyMap_.find(id);

		if (foundPolygond == polyMap_.end())
		{
			throw std::runtime_error(boost::str(boost::format("Trying to fetch unknow polygon id=%d from locator") % id));
		}
	
		if (bg::intersects(requestedPolygon, ConvertToBGPolygon2d(foundPolygond->second)))
		{
			resultIds.push_back(id);
		}
	});

	return resultIds;
}
