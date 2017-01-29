#pragma once

#include "Matrix.h"
#include "Polygon3d.h"

#include <vector>

namespace trm
{
	bool CheckPolygonIsVisible(const trm::Matrix & pv, const trm::Polygon3d & points);
	Polygon3d GetConvertedPolygon4PointsAndLess(const trm::Matrix & pv, const trm::Polygon3d & points);
	Polygon3d GetConvertedPolygonGeneric(const trm::Matrix & pv, const trm::Polygon3d & points);

} // namespace trm
