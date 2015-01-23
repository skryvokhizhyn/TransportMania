#pragma once

#include "ResourceProxy.h"
#include "ModelManager.h"

namespace trm
{
	using ModelManagerProxy = utils::ResourceProxy<ModelManager, utils::StaticRefHoldingStrategy>;

} // namespace trm
