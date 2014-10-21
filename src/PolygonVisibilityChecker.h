#pragma once

#include "Matrix.h"

#include <vector>

namespace trm
{
	struct Point3d;

	using Polygon3d = std::vector<Point3d>;

	bool CheckPolygonIsVisible(const trm::Matrix & pv, const trm::Polygon3d & points);

} // namespace trm
