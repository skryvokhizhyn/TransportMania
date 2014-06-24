#include "TerrainRangeCircle.h"
#include "Point2d.h"

using namespace trm;

namespace
{
	int GetLeftBorder(const int i, const size_t radii)
	{
		if (radii == 0)
			return 0;

		int j = -1;

		while(sqrt(static_cast<float>(i * i + j * j)) < radii)
		{
			--j;
		}

		return j;
	}
}

TerrainRangeCircle::TerrainRangeCircle(const Point2d & center, const AxisType radii)
{
	assert(radii >= 0.0f);

	const size_t r = static_cast<size_t>(ceil(radii));

	Init(r * 2 + 1);

	const int cx = static_cast<int>(center.x());
	const int cy = static_cast<int>(center.y());

	int y = -1 * r;

	while(y <= radii)
	{
		const int xB = GetLeftBorder(y, r);
		const int xE = ::abs(xB);
	
		PutRange(Range(y + cy, xB + cx, xE + cx));

		++y;
	}
}