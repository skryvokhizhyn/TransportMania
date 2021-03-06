#include "UpdateRate.h"

using namespace trm;

// update rate is controlled by vsync frequency which is 60 (?)
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
UpdateRate::NeedMore() const
{
	return (end_ - start_) >= updateDuration_;
}

void
UpdateRate::Done()
{
	++cnt_;
	start_ += updateDuration_;
}