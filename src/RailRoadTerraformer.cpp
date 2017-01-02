#include "RailRoadTerraformer.h"
#include "RailRoadArc.h"
#include "RailRoadLine.h"

using namespace trm;

void 
RailRoadTerraformer::Visit(RailRoadArc & rra)
{
	terraformer_ = TerraformFunctionFactory::GetSpiral(rra.GetSpiral());
}

void 
RailRoadTerraformer::Visit(RailRoadLine & rrl)
{
	terraformer_ = TerraformFunctionFactory::GetLinear(rrl.GetStart(), rrl.GetEnd());
}

const TerraformFunctionPtr &
RailRoadTerraformer::GetTerraformer() const
{
	return terraformer_;
}
