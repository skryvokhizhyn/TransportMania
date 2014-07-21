#pragma once

#include <boost/noncopyable.hpp>

#include <chrono>

namespace trm
{
	class UpdateRate
		: boost::noncopyable
	{
	public:
		UpdateRate(const unsigned updatesPerSecond);

		void Tick();

		bool NeedMore();

	private:
		const std::chrono::milliseconds updateDuration_;
		std::chrono::steady_clock::time_point start_;
		std::chrono::steady_clock::time_point end_;
	};

} // namespace trm
