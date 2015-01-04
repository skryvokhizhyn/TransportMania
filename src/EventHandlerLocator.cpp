#include "EventHandlerLocator.h"
#include "Logger.h"

//assignment operator could not be generated
#pragma warning(disable: 4512)

#include <boost/assign/std/vector.hpp>

#include <boost/range/algorithm/for_each.hpp>

using namespace trm;

void
EventHandlerLocator::Put(const int id, const Polygon2d & area, EventHandlerPtr ehPtr)
{
	locator_.Put(id, area);
	handlers_.emplace(id, std::move(ehPtr));
}

void
EventHandlerLocator::Remove(const int id)
{
	handlers_.erase(id);
	locator_.Remove(id);
}

EventHandler *  
EventHandlerLocator::GetHandler(const Point2d & p) const
{
	using namespace boost::assign;

	Polygon2d polygon;
	polygon.reserve(4);
	const float shift = 5.0f;

	polygon += p + Point2d(-shift, -shift),
		p + Point2d(-shift, shift),
		p + Point2d(shift, shift),
		p + Point2d(shift, -shift);

	const ItemLocator::Ids ids = locator_.At(polygon);
	
	if (ids.empty())
	{
		return nullptr;
	}

	return handlers_.at(ids.back()).get();
}

template<typename Event>
void 
EventHandlerLocator::DispatchImpl(const Event & e) const
{
	if (EventHandler * pHandler = GetHandler(e.pos))
	{
		pHandler->Process(e);
	}
	else
	{
		utils::Logger().Debug() << "No window handler found in pos=" << e.pos << ". Ignoring event";
	}
}

void
EventHandlerLocator::Dispatch(const FingerPressed & e) const
{
	DispatchImpl(e);
}

void
EventHandlerLocator::Dispatch(const FingerReleased & e) const
{
	DispatchImpl(e);
}

void
EventHandlerLocator::Dispatch(const FingerMoved & e) const
{
	DispatchImpl(e);
}


