#pragma once

#include "Polygon2d.h"
#include "EventHandler.h"
#include "ItemLocator.h"

#include <unordered_map>

namespace trm
{
	enum class HandlerLevel
	{
		Default = 0,
		Terrain = 1,
		Button = 2
	};

	class EventHandlerLocator
	{
	public:
		void Put(UniqueId id, const Polygon2d & area, HandlerLevel level, EventHandlerPtr ehPtr);
		void Remove(UniqueId id);

		EventHandler * At(const Point2d & p) const;

	private:
		struct HandlerDetails
		{
			HandlerLevel level;
			EventHandlerPtr handlerPtr;

			HandlerDetails(HandlerLevel l, EventHandlerPtr h);
		};

		using Handlers = std::unordered_map<UniqueId, HandlerDetails>;

	private:
		ItemLocator locator_;
		Handlers handlers_;
	};

} // namespace trm
