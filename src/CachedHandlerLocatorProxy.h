#pragma once

#include "ResourceProxy.h"
#include "CachedHandlerLocator.h"

namespace trm
{
	using CachedHandlerLocatorProxy = utils::ResourceProxy<CachedHandlerLocator, utils::StaticRefHoldingStrategy>;

} // namespace trm
