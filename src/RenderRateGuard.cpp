#include "RenderRateGuard.h"

#include <thread>

using namespace trm;

RenderRateGuard::RenderRateGuard(unsigned rate)
	: renderDuration_(std::chrono::milliseconds(1000) / rate)
{
	end_ = std::chrono::steady_clock::now() + renderDuration_;
}

void
RenderRateGuard::Snooze()
{
	const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	const std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(end_ - now);
	
	// sleep for the rest of the time slot
	if (diff.count() > 0)
	{
		std::this_thread::sleep_for(diff);
		// means that we're able to keep the render rate at a good level
		end_ = end_ + renderDuration_;
	}
	else
	{
		// we cannot make the render rate quick enough so we shift to "now"
		// rather than to the set up end_
		end_ = now + renderDuration_;
	}
}