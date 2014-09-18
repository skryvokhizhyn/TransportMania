#pragma once

#include "ResourceProxy.h"
#include "ComponentHolder.h"

namespace trm
{
	using ComponentHolderProxy = utils::ResourceProxy<ComponentHolder, utils::StaticRefHoldingStrategy>;

} // namespace trm
