#pragma once

#include "Train.h"
#include "RoadPoint.h"

#include <boost/range/algorithm/for_each.hpp>
#include <boost/bind.hpp>

namespace trm
{
	using PositionWPtr = std::weak_ptr<Point3d>;

	class TrainMovableObject
	{
	public:
		TrainMovableObject(RoadPoint rp, TrainPartType type);

		void Move(const float dist);

		const Point3d & Position() const;
		Point3d MovedPosition(const float dist) const;

		TrainPartType Type() const;

		bool GetVisible() const;
		void SetVisible(bool v);

		PositionWPtr SharedPosition() const;

	private:
		using PositionSPtr = std::shared_ptr<Point3d>;

	private:
		RoadPoint roadPoint_;
		TrainPartType type_;
		PositionSPtr sharedPosition_;
		bool visible_;
	};

} // namespace trm
