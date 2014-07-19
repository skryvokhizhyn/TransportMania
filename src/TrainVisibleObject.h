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

		const Matrix & GetMatrix() const;

		void Draw() const;

	private:
		void CalculateMatrixes(const Point3d & from, const Point3d & to);

	private:
		ModelDrawer drawer_;
		Matrix matrix_;
		PositionWPtr sourcePosition_;
		Point3d position_;
	};

} // namespace trm
