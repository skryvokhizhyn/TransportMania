#pragma once

#include "RailRoadVisitor.h"
#include "TerrainRange.h"

namespace trm
{
	class RailRoadRangeGenerator
		: public RailRoadVisitor
	{
	public:
		RailRoadRangeGenerator(bool useFullWidth);

		virtual void Visit(RailRoadArc & rra) override;
		virtual void Visit(RailRoadLine & rrl) override;

		const TerrainRange & GetRange();

	private:
		TerrainRange range_;
		const bool useFullWidth_;
	};

} // namespace trm
