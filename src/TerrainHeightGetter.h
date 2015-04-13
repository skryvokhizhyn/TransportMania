#pragma once

#include "Point2d.h"
#include "TerraformFunction.h"
#include "Polygon3d.h"

namespace trm
{
	struct Point2d;

	class TerrainHeightGetter
		: public TerraformFunction
	{
	public:
		bool operator () (const Point2d & p, float & height);

		const Polygon3d & Get() const;

	private:
		Polygon3d points_;
	};

} // namespace trm
