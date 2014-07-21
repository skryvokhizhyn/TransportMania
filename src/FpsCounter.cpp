#include "FpsCounter.h"

#include <boost/numeric/conversion/cast.hpp>

#include <assert.h>
#include <cmath>

using namespace trm;
using namespace std::chrono;

FpsCounter::FpsCounter(const unsigned reportPerSeconds)
	: reportDuration_(reportPerSeconds)
	, frames_(0.0f)
{
	begin_ = steady_clock::now();
}

bool
FpsCounter::Tick()
{
	assert(begin_ != steady_clock::time_point());

	frames_ += 1.0f;

	end_ = steady_clock::now();
	const steady_clock::duration dur = end_ - begin_;

	return dur > reportDuration_;
}

unsigned
FpsCounter::GetFrames()
{
	const steady_clock::duration dur = end_ - begin_;
	const int secondsPassed = duration_cast<duration<int>>(dur).count();

	const float framesRateFloat = frames_ / secondsPassed;

	float intPart = 0.0f;
	// save fractional part in the counter of frames
	frames_ = std::modf(framesRateFloat, &intPart);

	begin_ = end_;

	return boost::numeric_cast<unsigned>(intPart);
}
