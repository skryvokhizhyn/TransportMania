#pragma once

#include "StaticSceneObject.h"
#include "RailRoad.h"

namespace trm
{
	struct StaticSceneObjectFactory
	{
		static StaticSceneObjectPtr ForRailRoad(const RailRoadPtr & rrp);
	};

} // namespace trm
