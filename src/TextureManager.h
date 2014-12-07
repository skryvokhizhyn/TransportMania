#pragma once

#include "TextureId.h"
#include "TextureProperties.h"

#include <map>

namespace trm
{
	class TextureManager
	{
	public:
		~TextureManager();

		const TextureProperties & Get(TextureId id);
		void Flush();

	private:
		using TextureCache = std::map<TextureId, TextureProperties>;

	private:
		TextureCache cache_;
	};

} // namespace trm
