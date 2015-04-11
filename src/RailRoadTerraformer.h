#pragma once

#include "RailRoadVisitor.h"
#include "TerraformFunction.h"

namespace trm
{
	class RailRoadTerraformer
		: public RailRoadVisitor
	{
	public:
		virtual void Visit(RailRoadArc & rra) override;
		virtual void Visit(RailRoadLine & rrl) override;

		// moves the terraformer out
		TerraformFunctionPtr GetTerraformer();

	private:
		TerraformFunctionPtr terraformer_;
	};

} // namespace trm
