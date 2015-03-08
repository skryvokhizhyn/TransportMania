#pragma once

#include "EventHandlerLocator.h"

#include <set>
#include <map>

namespace trm
{
	class CachedHandlerLocator
	{
	public:
		// mimic EventHandlerLocator
		void Put(UniqueId id, const Polygon2d & area, HandlerLevel level, EventHandlerPtr ehPtr);
		void Remove(UniqueId id);

		// mimic EventHandler
		void Process(const FingerPressed & e);
		void Process(const FingerReleased & e);
		void Process(const FingerMoved & e);

	private:
		template<typename Event>
		void DispatchImpl(const Event & e, bool lockHandler);

	private:
		using PressedFingers = std::set<FingerId>;
		using IdToHandlerMap = std::map<UniqueId, EventHandlerPtr>;

	private:
		EventHandlerLocator ehLocator_;
		PressedFingers fingersPressed_;
		IdToHandlerMap idToHandlerMap_;
		EventHandler * activeHandler_ = nullptr;
	};

} // namespace trm
