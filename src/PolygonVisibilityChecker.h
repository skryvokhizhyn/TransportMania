#pragma once

#include "Matrix.h"
#include "Polygon3d.h"

#include <vector>

namespace trm
{
	bool CheckPolygonIsVisible(const trm::Matrix & pv, const trm::Polygon3d & points);

} // namespace trm
