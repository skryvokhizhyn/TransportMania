#include "TextureManager.h"
#include "TextureLoader.h"
#include "Settings.h"

#include <boost/assign/list_of.hpp>
#include <boost/range/algorithm/for_each.hpp>

using namespace trm;

namespace
{
	using TextureIdToFileMap = std::map<TextureId, std::string>;
	const TextureIdToFileMap TEXTURE_ID_TO_FILE_MAP = boost::assign::map_list_of
		(TextureId::Ground,		"grass.png")
		(TextureId::Font,		"arial_ttf_cyr_lat_0.png")
		(TextureId::OkButton,	"okbutton.png");

	std::string GetTextureLocation(TextureId id)
	{
		auto found = TEXTURE_ID_TO_FILE_MAP.find(id);
		if (found == TEXTURE_ID_TO_FILE_MAP.end())
		{
			throw std::runtime_error(boost::str(boost::format("No mapping for texture # %d") % static_cast<int>(id)));
		}

		return trm::GetTexturePath(found->second);
	}
}

TextureManager::~TextureManager()
{
	Flush();
}

const TextureProperties &
TextureManager::Get(TextureId id)
{
	auto found = cache_.lower_bound(id);
	if (found == cache_.end() || id != found->first)
	{
		const auto props = TextureLoader::Load(GetTextureLocation(id));

		found = cache_.emplace_hint(found, id, props);
	}

	return found->second;
}

void
TextureManager::Flush()
{
	boost::for_each(cache_, 
		[](const TextureCache::value_type & v)
	{
		TextureLoader::Delete(v.second.id);
	});

	cache_.clear();
}