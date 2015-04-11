#pragma once

#include "RailRoadVisitor.h"
#include "TerrainRange.h"

namespace trm
{
	class RailRoadRangeGenerator
		: public RailRoadVisitor
	{
	public:
		virtual void Visit(RailRoadArc & rra) override;
		virtual void Visit(RailRoadLine & rrl) override;

		const TerrainRange & GetRange();

	private:
		TerrainRange range_;
	};

} // namespace trm
