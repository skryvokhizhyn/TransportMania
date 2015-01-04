#pragma once

#include "EventHandler.h"

#include <functional>

namespace trm
{
	using OnEventCallback = std::function<void ()>;

	class EventHandlerWrapper
		: public EventHandler
	{
	public:
		EventHandlerWrapper(OnEventCallback cb)
			: cb_(cb)
		{}

		// EventHandler
		virtual void Process(const FingerPressed & /*e*/) override
		{
			if (cb_)
				cb_();
		}
		
		virtual void Process(const FingerReleased & /*e*/) override {}
		virtual void Process(const FingerMoved & /*e*/) override {}
		virtual void Reset() override {}

	private:
		OnEventCallback cb_;
	};

} // namespace trm
