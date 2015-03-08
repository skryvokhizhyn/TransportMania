#include "CachedHandlerLocator.h"
#include "Logger.h"

using namespace trm;

void
CachedHandlerLocator::Put(UniqueId id, const Polygon2d & area, HandlerLevel level, EventHandlerPtr ehPtr)
{
	ehLocator_.Put(id, area, level, ehPtr);
	idToHandlerMap_.emplace(id, ehPtr);
}

void
CachedHandlerLocator::Remove(UniqueId id)
{
	ehLocator_.Remove(id);
	EventHandlerPtr ehPtr = idToHandlerMap_.at(id);
	
	// check if we remove active handler and reset it if so
	if (ehPtr.get() == activeHandler_)
	{
		activeHandler_ = nullptr;
	}

	idToHandlerMap_.erase(id);
}

void
CachedHandlerLocator::Process(const FingerPressed & e)
{
	DispatchImpl(e, true);
	fingersPressed_.insert(e.fingerId);
}

void
CachedHandlerLocator::Process(const FingerReleased & e)
{
	DispatchImpl(e, false);
	fingersPressed_.erase(e.fingerId);

	// reset active handler when all fingers released
	if (fingersPressed_.empty())
	{
		activeHandler_ = nullptr;
	}
}

void
CachedHandlerLocator::Process(const FingerMoved & e)
{
	DispatchImpl(e, false);
}

template<typename Event>
void 
CachedHandlerLocator::DispatchImpl(const Event & e, bool lockHandler)
{
	EventHandler * pCurrentHandler = nullptr;

	// we keep on using active hanldel until null handler is found (e.i. out of visible area)
	// and untill we have at least one finger pressed
	{
		EventHandler * pLocatedHandler = ehLocator_.At(e.pos);

		if (lockHandler)
		{
			if (activeHandler_ == nullptr)
			{
				activeHandler_ = pLocatedHandler;
			}
			else
			{
				if (pLocatedHandler == nullptr)
				{
					activeHandler_->Reset();
					activeHandler_ = nullptr;
				}
			}

			pCurrentHandler = activeHandler_;
		}
		else
		{
			if (activeHandler_ == nullptr)
			{
				pCurrentHandler = pLocatedHandler;
			}
			else
			{
				pCurrentHandler = activeHandler_;
			}
		}
	}

	if (pCurrentHandler)
	{
		pCurrentHandler->Process(e);
	}
	else
	{
		utils::Logger().Debug() << "No window handler found at pos=" << e.pos << ". Ignoring event";
	}
}
