#include "RailRoadTerraformer.h"
#include "RailRoadArc.h"
#include "RailRoadLine.h"

using namespace trm;

void 
RailRoadTerraformer::Visit(RailRoadArc & rra)
{
	terraformer_ = TerraformFunctionFactory::GetConstant(rra.GetStart().z());
}

void 
RailRoadTerraformer::Visit(RailRoadLine & rrl)
{
	terraformer_ = TerraformFunctionFactory::GetLinear(rrl.GetStart(), rrl.GetEnd());
}

TerraformFunction 
RailRoadTerraformer::GetTerraformer()
{
	return std::move(terraformer_);
}
