#pragma once

#include "Polygon2d.h"
#include "EventHandler.h"
#include "ItemLocator.h"

#include <unordered_map>

namespace trm
{
	class EventHandlerLocator
	{
	public:
		void Put(UniqueId id, const Polygon2d & area, EventHandlerPtr ehPtr);
		void Remove(UniqueId id);

		EventHandler * At(const Point2d & p) const;

	private:
		using Handlers = std::unordered_map<UniqueId, EventHandlerPtr>;

	private:
		ItemLocator locator_;
		Handlers handlers_;
	};

} // namespace trm
