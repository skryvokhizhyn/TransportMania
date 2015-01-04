#pragma once

#include "FingerEvent.h"

#include <boost/noncopyable.hpp>

namespace trm
{
namespace impl
{
	// Events

	struct Commit {};

	struct PointMove
	{
		trm::Point2d from;
		trm::Point2d to;

		PointMove() {}
		explicit PointMove(const trm::Point2d & f)
			: PointMove(f, f)
		{}
		PointMove(const trm::Point2d & f, const trm::Point2d & t)
			: from(f), to(t)
		{}

		bool Valid() const
		{ return from != to; }
	};

} // namespace impl
} // namespace trm
