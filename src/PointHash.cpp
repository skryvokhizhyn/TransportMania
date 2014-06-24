#include "PointHash.h"
#include "Point3d.h"
#include <boost/functional/hash.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace
{
	int RoundCoordinate(const trm::Point3d::Type v)
	{
		// the higher the better
		const size_t precision = 100;

		return boost::numeric_cast<int>(v * precision);
	}
}

size_t 
trm::hash_value(const Point3d & p)
{
	size_t seed = 0;

	boost::hash_combine(seed, RoundCoordinate(p.x()));
	boost::hash_combine(seed, RoundCoordinate(p.y()));
	boost::hash_combine(seed, RoundCoordinate(p.z()));

	return seed;
}