#pragma once

#include "ModelData.h"
#include "TerraformFunction.h"

namespace trm
{
	class TerrainPointCollector
		: public TerraformFunction
	{
	public:
		bool operator () (const Point2d & p, float & height);

		const PointVector & GetPoints() const;

	private:
		PointVector points_;
	};

} // namespace trm

