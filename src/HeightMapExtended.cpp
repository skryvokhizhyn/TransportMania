#include "HeightMapExtended.h"
#include "Point2d.h"
#include <boost/format.hpp>

using namespace trm;
using namespace trm::terrain;

namespace
{
	HeightMap::Type Interpolate(const HeightMap::Type f0, const HeightMap::Type f1,
		const float x0, const float x1, const float x)
	{
		if (utils::CheckEqual(x0, x1))
		{
			return f0;
		}

		if (utils::CheckEqual(x, x0))
		{
			return f0;
		}

		if (utils::CheckEqual(x, x1))
		{
			return f1;
		}

		return f0 + (f1 - f0) / (x1 - x0) * (x - x0);
	}

}

HeightMapExtended::HeightMapExtended(const size_t sz)
: extendedSz_(sz)
{
}

HeightMap::Type
HeightMapExtended::At(const Point2d & p) const
{
	if (p.x() >= extendedSz_ || p.y() >= extendedSz_)
	{
		const std::string err = (boost::format("Requested value out of range. Sz = %f x = %f y = %f") % extendedSz_ % p.x() % p.y()).str();
		throw std::range_error(err);
	}

	if (extendedSz_ == GetSize())
	{
		return HeightMap::At(p);
	}

	const float factor = static_cast<float>(extendedSz_) / GetSize();

	const float xScaled = p.x() / factor;
	const float yScaled = p.y() / factor;

	const float xUp = ::ceil(xScaled);
	const float xDown = ::floor(xScaled);
	const float yUp = ::ceil(yScaled);
	const float yDown = ::floor(yScaled);

	const HeightMap::Type v1U = HeightMap::At(Point2d(xUp, yDown));
	const HeightMap::Type v2U = HeightMap::At(Point2d(xUp, yUp));

	const HeightMap::Type v1 = Interpolate(v1U, v2U, yDown, yUp, yScaled);

	const HeightMap::Type v1D = HeightMap::At(Point2d(xDown, yDown));
	const HeightMap::Type v2D = HeightMap::At(Point2d(xDown, yUp));

	const HeightMap::Type v2 = Interpolate(v1D, v2D, yDown, yUp, yScaled);

	const HeightMap::Type v = Interpolate(v1, v2, xDown, xUp, xScaled);

	return v;
}

void 
HeightMapExtended::Set(const Point2d & /*p*/, const Type )
{
	throw std::runtime_error("Not implemented");
}