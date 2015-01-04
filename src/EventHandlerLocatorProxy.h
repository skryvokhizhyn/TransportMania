#pragma once

#include "ResourceProxy.h"
#include "EventHandlerLocator.h"

namespace trm
{
	using EventHandlerLocatorProxy = utils::ResourceProxy<EventHandlerLocator, utils::StaticRefHoldingStrategy>;

} // namespace trm
