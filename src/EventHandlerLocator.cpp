#include "EventHandlerLocator.h"
#include "Logger.h"

//assignment operator could not be generated
#pragma warning(disable: 4512)

#include <boost/assign/std/vector.hpp>

#include <boost/range/algorithm/sort.hpp>

using namespace trm;

void
EventHandlerLocator::Put(UniqueId id, const Polygon2d & area, EventHandlerPtr ehPtr)
{
	locator_.Put(id, area);
	handlers_.emplace(id, std::move(ehPtr));
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

	// assumption is that the bigger id is, the closer locator to the view
	boost::sort(ids);

	return handlers_.at(ids.back()).get();
}
