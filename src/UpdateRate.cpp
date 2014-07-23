#include "UpdateRate.h"

using namespace trm;

UpdateRate::UpdateRate(const unsigned updatesPerSecond)
	: updateDuration_(std::chrono::milliseconds(1000) / updatesPerSecond)
	, cnt_(0)
{
	start_ = std::chrono::steady_clock::now();
	// assume we have exactly 1/30 second for the cycle
	// it will normalize itself during next cycles
	end_ = start_ + updateDuration_;
}

unsigned
UpdateRate::Tick()
{
	const unsigned cnt = cnt_;
	cnt_ = 0;
	
	end_ = std::chrono::steady_clock::now();

	return cnt;
}

bool
UpdateRate::NeedMore()
{
	if ((end_ - start_) >= updateDuration_)
	{
		++cnt_;
		start_ += updateDuration_;

		return true;
	}
	
	return false;
}