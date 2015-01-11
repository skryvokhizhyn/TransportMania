#pragma once

#include "ResourceProxy.h"
#include "TextRenderer.h"

namespace trm
{
	using TextRendererProxy = utils::ResourceProxy<TextRenderer, utils::StaticRefHoldingStrategy>;

} // namespace trm
