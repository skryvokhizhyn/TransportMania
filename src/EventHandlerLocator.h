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

		void Dispatch(const FingerPressed & e) const;
		void Dispatch(const FingerReleased & e) const;
		void Dispatch(const FingerMoved & e) const;

	private:
		EventHandler * GetHandler(const Point2d & p) const;
		template<typename Event>
		void DispatchImpl(const Event & e) const;

	private:
		using Handlers = std::unordered_map<int, EventHandlerPtr>;

	private:
		ItemLocator locator_;
		Handlers handlers_;
	};

} // namespace trm
