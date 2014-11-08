#pragma once

#include "ModelDrawer.h"
#include "Matrix.h"
#include "TrainTypes.h"
#include "TrainMovableObject.h"

namespace trm
{
	class TrainVisibleObject
	{
	public:
		TrainVisibleObject(const TrainPartType type, PositionWPtr posPtr, const Point3d & nextPos);

		bool Update();

		Matrix GetMatrix() const;

		void Draw() const;

	private:
		PositionWPtr sourcePosition_;
		Point3d positionFrom_;
		Point3d positionTo_;
		ModelDrawer drawer_;
	};

} // namespace trm
