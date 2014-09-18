#pragma once

#include "ComponentId.h"
#include "TrainMovableObject.h"
#include "TrainVisibleObject.h"
#include "Train.h"
#include <map>
#include <vector>

namespace trm
{
	class WorldProjection;
	class RoadPoint;
	class DrawContext;

	class ComponentHolder
	{
	public:
		void Update(const WorldProjection & wp);
		void PutTrain(ComponentId id, Train && train, RoadPoint rp);
		void Move(ComponentId id, float dist);
		void Remove(ComponentId id);
		void Draw(const DrawContext & c, const Matrix & projectionViewMatrix) const;

	private:
		void UpdateVisible(const WorldProjection & wp, TrainMovableObject & to);

	private:
		using Trains = std::map<ComponentId, Train>;
		using Movables = std::multimap<ComponentId, TrainMovableObject>;
		using Visibles = std::vector<TrainVisibleObject>;

		Trains trains_;
		Movables movables_;
		Visibles visibles_;
	};

} // namespace trm
