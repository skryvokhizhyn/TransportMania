#pragma once

#include <cstddef>

namespace trm
{
	struct Point3d;
	struct Point3i;

	size_t hash_value(const Point3d & p);
	size_t hash_value(const Point3i & p);

} // namespace trm
