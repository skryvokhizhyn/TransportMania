#pragma once

#include <chrono>
#include <cstdint>

namespace trm
{
	class RenderRateGuard
	{
	public:
		RenderRateGuard(unsigned rate);

		std::uint8_t Snooze();

	private:
		const std::chrono::milliseconds renderDuration_;
		std::chrono::steady_clock::time_point end_;
	};

} // namespace trm
