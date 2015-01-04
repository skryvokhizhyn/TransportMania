#pragma once 

#include "FingerEvent.h"

#include <memory>

namespace trm
{
	struct EventHandler
	{
		virtual ~EventHandler() {}

		virtual void Process(const FingerPressed & e) = 0;
		virtual void Process(const FingerReleased & e) = 0;
		virtual void Process(const FingerMoved & e) = 0;
	};

	using EventHandlerPtr = std::shared_ptr<EventHandler>;

} // namespace trm
