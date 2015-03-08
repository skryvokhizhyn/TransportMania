#pragma once

#include "StaticSceneObject.h"
#include "RailRoad.h"
#include "ModelData.h"

namespace trm
{
	struct StaticSceneObjectFactory
	{
		static StaticSceneObjectPtr ForRailRoad(const RailRoadPtr & rrp);
		static StaticSceneObjectPtr ForTerrainCover(const PointVector & pv);
	};

} // namespace trm
