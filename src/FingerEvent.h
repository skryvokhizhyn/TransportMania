#pragma once

#include "Point2d.h"

#include <cstdint>

namespace trm
{
	using FingerId = std::int64_t;

	struct FingerPressed
	{
		FingerId fingerId;
		Point2d pos;

		FingerPressed(FingerId id, const Point2d & p) : fingerId(id), pos(p) {}
	};

	struct FingerMoved
	{
		FingerId fingerId;
		Point2d pos;

		FingerMoved(FingerId id, const Point2d & p) : fingerId(id), pos(p) {}
	};

	struct FingerReleased
	{
		FingerId fingerId;
		Point2d pos;

		FingerReleased(FingerId id, const Point2d & p) : fingerId(id), pos(p) {}
	};

} // namespace trm
