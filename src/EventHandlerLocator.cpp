#include "EventHandlerLocator.h"
#include "Logger.h"

//assignment operator could not be generated
#pragma warning(disable: 4512)

#include <boost/assign/std/vector.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/sort.hpp>

using namespace trm;

EventHandlerLocator::HandlerDetails::HandlerDetails(HandlerLevel l, EventHandlerPtr h)
	: level(l)
	, handlerPtr(h)
{}

void
EventHandlerLocator::Put(UniqueId id, const Polygon2d & area, HandlerLevel level, EventHandlerPtr ehPtr)
{
	locator_.Put(id, area);
	handlers_.emplace(id, HandlerDetails(level, ehPtr));
}

void
EventHandlerLocator::Remove(UniqueId id)
{
	handlers_.erase(id);
	locator_.Remove(id);
}

EventHandler * 
EventHandlerLocator::At(const Point2d & p) const
{
	using namespace boost::assign;

	Polygon2d polygon;
	polygon.reserve(4);
	const float shift = 5.0f;

	polygon += p + Point2d(-shift, -shift),
		p + Point2d(-shift, shift),
		p + Point2d(shift, shift),
		p + Point2d(shift, -shift);

	ItemLocator::Ids ids = locator_.At(polygon);
	
	if (ids.empty())
	{
		return nullptr;
	}

	using SortingVector = std::vector<boost::tuple<HandlerLevel, UniqueId>>;
	SortingVector sortingContainer;
	sortingContainer.reserve(ids.size());

	boost::for_each(ids, 
		[&](const UniqueId & id)
	{
		sortingContainer.push_back(boost::make_tuple(handlers_.at(id).level, id));
	});

	boost::sort(sortingContainer);

	return handlers_.at(sortingContainer.back().get<1>()).handlerPtr.get();
}
