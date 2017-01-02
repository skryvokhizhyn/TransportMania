#include "PointHash.h"
#include "Point3d.h"
#include "Point3i.h"
#include "Size2d.h"
#include <boost/functional/hash.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace
{
	template<typename T>
	int RoundCoordinate(T v)
	{
		// the higher the better
		const size_t precision = 100;

		return boost::numeric_cast<int>(v * precision);
	}

	template<typename T>
	size_t hash_value_impl_3d(const T & p)
	{
		size_t seed = 0;

		boost::hash_combine(seed, RoundCoordinate(p.x()));
		boost::hash_combine(seed, RoundCoordinate(p.y()));
		boost::hash_combine(seed, RoundCoordinate(p.z()));

		return seed;
	}

	template<typename T>
	size_t hash_value_impl_2d(const T & p)
	{
		size_t seed = 0;

		boost::hash_combine(seed, RoundCoordinate(p.x()));
		boost::hash_combine(seed, RoundCoordinate(p.y()));

		return seed;
	}
}

size_t trm::hash_value(const Point3d & p)
{
	return hash_value_impl_3d(p);
}

size_t trm::hash_value(const Point3i & p)
{
	return hash_value_impl_3d(p);
}

size_t trm::hash_value(const Size2d & s)
{
	return hash_value_impl_2d(s);
}