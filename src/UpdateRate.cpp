#include "UpdateRate.h"

using namespace trm;

UpdateRate::UpdateRate(const unsigned updatesPerSecond)
	: updateDuration_(std::chrono::milliseconds(1000) / updatesPerSecond)
{
	start_ = std::chrono::steady_clock::now();
	// assume we have exactly 1/30 second for the cycle
	// it will normalize itself during next cycles
	end_ = start_ + updateDuration_;
}

void
UpdateRate::Tick()
{
	end_ = std::chrono::steady_clock::now();
}

bool
UpdateRate::NeedMore()
{
	if ((end_ - start_) >= updateDuration_)
	{
		start_ += updateDuration_;

		return true;
	}
	
	return false;
}