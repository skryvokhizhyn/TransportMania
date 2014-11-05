#include "HeightMapExtended.h"
#include "Point2d.h"
#include <boost/format.hpp>

using namespace trm;
using namespace trm::terrain;

namespace
{
	HeightMap::Value Interpolate(const HeightMap::Value f0, const HeightMap::Value f1,
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

HeightMap::Value
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

	const Value v1U = HeightMap::At(Point2d(xUp, yDown));
	const Value v2U = HeightMap::At(Point2d(xUp, yUp));

	const Value v1 = Interpolate(v1U, v2U, yDown, yUp, yScaled);

	const Value v1D = HeightMap::At(Point2d(xDown, yDown));
	const Value v2D = HeightMap::At(Point2d(xDown, yUp));

	const Value v2 = Interpolate(v1D, v2D, yDown, yUp, yScaled);

	const Value v = Interpolate(v1, v2, xDown, xUp, xScaled);

	return v;
}

void 
HeightMapExtended::Set(const Point2d & /*p*/, const Type )
{
	throw std::runtime_error("Not implemented");
}