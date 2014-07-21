#pragma once

#include <boost/noncopyable.hpp>

#include <chrono>

namespace trm
{
	class FpsCounter
		: boost::noncopyable
	{
	public:
		FpsCounter(const unsigned reportPerSeconds);

		bool Tick();
		unsigned GetFrames();

	private:
		const std::chrono::seconds reportDuration_;
		std::chrono::steady_clock::time_point begin_;
		std::chrono::steady_clock::time_point end_;
		float frames_;
	};

} // namespace trm
