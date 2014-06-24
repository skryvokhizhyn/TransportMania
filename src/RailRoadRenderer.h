#pragma once

#include "RailRoadVisitor.h"

namespace trm
{
	struct ModelData;

	class RailRoadRenderer
		: public RailRoadVisitor
	{
	public:
		RailRoadRenderer(ModelData & md);

		virtual void Visit(RailRoadLine & rrl) override;
		virtual void Visit(RailRoadArc & rra) override;

	private:
		ModelData & md_;
	};

} // namespace trm