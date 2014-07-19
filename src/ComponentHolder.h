#pragma once

#include "ComponentId.h"
#include "TrainMovableObject.h"
#include "TrainVisibleObject.h"
#include "Train.h"
#include <map>
#include <vector>

namespace trm
{
	struct ComponentHolder
	{
		using Trains = std::map<ComponentId, Train>;
		using Movables = std::multimap<ComponentId, TrainMovableObject>;
		using Visibles = std::vector<TrainVisibleObject>;

		Trains trains;
		Movables movables;
		Visibles visibles;
	};

} // namespace trm
