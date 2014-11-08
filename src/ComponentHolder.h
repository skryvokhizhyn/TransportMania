#pragma once

#include "TrainMovableObject.h"
#include "TrainVisibleObject.h"
#include "Train.h"
#include <vector>
#include <list>

namespace trm
{
	class WorldProjection;
	class RoadPoint;
	class DrawContext;

	class ComponentHolder
	{
	private:
		using Movables = std::vector<TrainMovableObject>;

		struct TrainData
		{
			Train train;
			Movables movables;
		};
				
		using Trains = std::list<TrainData>;

	public:
		using TrainIt = Trains::iterator;

	public:
		void Update(const WorldProjection & wp);
		TrainIt PutTrain(Train && train, RoadPoint rp);
		void Move(TrainIt it, float dist);
		void Remove(TrainIt it);
		void Draw(const DrawContext & c, const Matrix & projectionViewMatrix) const;

	private:
		void UpdateVisible(const WorldProjection & wp, TrainMovableObject & to);

	private:
		using Visibles = std::vector<TrainVisibleObject>;

		Trains trains_;
		Visibles visibles_;
	};

} // namespace trm
