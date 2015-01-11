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

#include <boost/range/algorithm/transform.hpp>

#include <unordered_map>

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
	Box2d ConvertToBox2d(const Polygon2d & polygon)
	{
		BGPolygon2d polygon2d;
		polygon2d.outer().reserve(polygon.size() + 1);

		boost::transform(polygon, std::back_inserter(polygon2d.outer()),
			[](const Point2d & p)
		{
			return bg::make<BGPoint2d>(p.x(), p.y());
		});

		bg::correct(polygon2d);

		return bg::return_envelope<Box2d>(polygon2d);
	}
}

class ItemLocator::ItemLocatorImpl
{
	using IdToBoxMap = std::unordered_map<UniqueId, Box2d>;
	using RTreeValue = IdToBoxMap::key_type;
	using MapIndexableType = priv::MapIndexableGetter<IdToBoxMap>;
	using CreationAlgo = bg::index::rstar<16, 4>;
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
	: implPtr_(std::make_shared<ItemLocatorImpl>())
{}

void 
ItemLocator::Put(UniqueId id, const Polygon2d & p)
{
	implPtr_->Put(id, p);
}

void 
ItemLocator::Remove(UniqueId id)
{
	implPtr_->Remove(id);
}

ItemLocator::Ids 
ItemLocator::At(const Polygon2d & p) const
{
	return implPtr_->At(p);
}
