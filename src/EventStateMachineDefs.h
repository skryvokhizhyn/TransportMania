#pragma once

#include "Point2d.h"

#include <boost/noncopyable.hpp>

#include <cstdint>

namespace trm
{
namespace impl
{
	// Events

	enum class MoveKeys { Left, Right, Up, Down };
	template<MoveKeys Key>
	struct MoveKeyPressed {};

	struct QuitFired {};
	struct Key1Pressed {};

	struct KeyQPressed {};
	struct KeyEPressed {};
	struct KeyZPressed {};
	struct KeyCPressed {};
	struct KeyRPressed {};
	struct KeyVPressed {};
	struct KeyTPressed {};
	struct KeyBPressed {};

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

	using FingerId = std::int64_t;

	struct FingerPressed
	{
		FingerId fingerId;
		trm::Point2d pos;

		FingerPressed(FingerId id, const Point2d & p) : fingerId(id), pos(p) {}
	};

	struct FingerMoved
	{
		FingerId fingerId;
		trm::Point2d pos;

		FingerMoved(FingerId id, const Point2d & p) : fingerId(id), pos(p) {}
	};

	struct FingerReleased
	{
		FingerId fingerId;

		FingerReleased(FingerId id) : fingerId(id) {}
	};

} // namespace impl
} // namespace trm
