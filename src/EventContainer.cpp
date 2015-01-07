#include "EventContainer.h"

#include <boost/range/algorithm/for_each.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using namespace trm;

EventHandlerPtr 
EventContainer::Create(EventActions actions, EventActionType type)
{
	return std::make_shared<EventContainer>(actions, type);
}

EventContainer::EventContainer(EventActions actions, EventActionType type)
	: actions_(std::move(actions))
	, type_(type)
{}

void
EventContainer::Process(const FingerPressed & /*e*/)
{
	if (actions_.empty())
	{
		return;
	}

	namespace bl = boost::lambda;

	boost::for_each(actions_, bl::bind(bl::_1));

	if (type_ == EventActionType::Single)
	{
		actions_.clear();
	}
}

void
EventContainer::Process(const FingerReleased & /*e*/)
{}

void
EventContainer::Process(const FingerMoved & /*e*/)
{}

void
EventContainer::Reset()
{}