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
		void Put(const int id, const Polygon2d & area, EventHandlerPtr ehPtr);
		void Remove(const int id);

		EventHandler * At(const Point2d & p) const;

	private:
		using Handlers = std::unordered_map<int, EventHandlerPtr>;

	private:
		ItemLocator locator_;
		Handlers handlers_;
	};

} // namespace trm
