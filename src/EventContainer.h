#pragma once

#include "EventHandler.h"
#include "EventAction.h"

#include <vector>

namespace trm
{
	class EventContainer
		: public EventHandler
	{
		using EventActions = std::vector<EventAction>;

	public:
		static EventHandlerPtr Create(EventActions actions, EventActionType type);

	public:
		EventContainer(EventActions actions, EventActionType type);

		// EventHandler
		virtual void Process(const FingerPressed & e) override;
		virtual void Process(const FingerReleased & e) override;
		virtual void Process(const FingerMoved & e) override;
		virtual void Reset() override;
		
	private:
		EventActions actions_;
		EventActionType type_;
	};

} // namespace trm
