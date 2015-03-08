#pragma once

#include "Terrain.h"
#include "OptionalPoint3d.h"

namespace trm
{
	class WorldProjection;
	class ScreenSizeManipulator;

	OptionalPoint3d GetTerrainPosition(
		const Point2d & p,
		const WorldProjection & wp, 
		const ScreenSizeManipulator & sm,
		const TerrainPtr & tPtr);

} // namespace trm
