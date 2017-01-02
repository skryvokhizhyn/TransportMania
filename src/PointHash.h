#pragma once

#include <cstddef>

namespace trm
{
	struct Point3d;
	struct Point3i;
	struct Size2d;

	size_t hash_value(const Point3d & p);
	size_t hash_value(const Point3i & p);
	size_t hash_value(const Size2d & s);

	template<typename T>
	struct Hasher
	{
		std::size_t operator()(const T & val) const
		{
			return hash_value(val);
		}
	};

} // namespace trm
