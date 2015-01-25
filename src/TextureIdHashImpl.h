#pragma once

#include "TextureId.h"

#include <functional>

namespace std
{
	template<>
	struct hash<trm::TextureId>
	{
		long operator() (trm::TextureId id) const
		{
			return hash<long>()(static_cast<long>(id));
		}
	};
}