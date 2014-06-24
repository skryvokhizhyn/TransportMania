#pragma once

#include "MovableObject.h"
#include "Train.h"
#include "RoadPoint.h"

namespace trm
{
	class TrainMovableObject
		: public MovableObject
	{
	public:
		TrainMovableObject(const RoadPoint & rp, const TrainWPtr & tPtr, const float initialShift);

		void Move() override;

	private:
		RoadPoint roadPoint_;
		const TrainWPtr trainPtr_;
	};

} // namespace trm
