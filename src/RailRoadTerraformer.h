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

		const TerraformFunctionPtr & GetTerraformer() const;

	private:
		TerraformFunctionPtr terraformer_;
	};

} // namespace trm
