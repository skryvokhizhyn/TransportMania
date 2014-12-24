#pragma once

#include "ResourceProxy.h"
#include "TextManager.h"

namespace trm
{
	using TextManagerProxy = utils::ResourceProxy<TextManager, utils::StaticRefHoldingStrategy>;

} // namespace trm
