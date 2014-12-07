#pragma once

#include "ResourceProxy.h"
#include "TextureManager.h"

namespace trm
{
	using TextureManagerProxy = utils::ResourceProxy<TextureManager, utils::StaticRefHoldingStrategy>;

} // namespace trm
