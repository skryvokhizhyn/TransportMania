#include "ModelManager.h"
#include "ModelReader.h"
#include "Settings.h"

#include <unordered_map>

#include <boost/assign/list_of.hpp>
#include <boost/range/algorithm/for_each.hpp>

using namespace trm;

namespace
{
	using TextureIdToFileMap = std::unordered_map<TextureId, std::string>;
	const TextureIdToFileMap TEXTURE_ID_TO_FILE_MAP = boost::assign::map_list_of
		(TextureId::Wagon,				"wagon.obj")
		(TextureId::Locomotive,			"locomotive.obj")
		;
}

ModelData
ModelManager::Get(TextureId id)
{
	auto found = TEXTURE_ID_TO_FILE_MAP.find(id);
	
	if (found == TEXTURE_ID_TO_FILE_MAP.end())
	{
		throw std::runtime_error((boost::format("Texture id=%d has no Obj file mapped") % static_cast<int>(id)).str());
	}

	ModelData md = ModelReader::Read(GetModelPath(found->second));
	md.type = ModelData::Mode::Triangle;
	md.textureId = id;

	return md;
}