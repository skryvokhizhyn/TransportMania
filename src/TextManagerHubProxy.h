#pragma once

#include "ResourceProxy.h"
#include "TextManagerHub.h"

namespace trm
{
	using TextManagerHubProxy = utils::ResourceProxy<TextManagerHub, utils::StaticRefHoldingStrategy>;

} // namespace trm
