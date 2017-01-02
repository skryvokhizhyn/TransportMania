#pragma once

#include <chrono>

namespace trm
{
	class RenderRateGuard
	{
	public:
		RenderRateGuard(unsigned rate);

		void Snooze();

	private:
		const std::chrono::milliseconds renderDuration_;
		std::chrono::steady_clock::time_point end_;
	};

} // namespace trm
