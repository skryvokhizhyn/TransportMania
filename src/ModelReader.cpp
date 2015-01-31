#include "ModelReader.h"

#include <boost/algorithm/string.hpp>

#include <fstream>
#include <map>

using namespace trm;

namespace
{
	using SplitResult = std::vector<std::string>;
	using Ids = std::tuple<int, int, int>;
	enum IdName
	{
		Point = 0,
		Texture = 1,
		Normale = 2
	};
	using IdsMap = std::map<Ids, int>;

	void RegisterVertex(const SplitResult & res, PointVector & vec)
	{
		if (res.size() != 4)
		{
			throw std::runtime_error("Unexpected coordinates number for vertex");
		}

		vec.emplace_back(
			boost::lexical_cast<float>(res[1]),
			boost::lexical_cast<float>(res[2]),
			boost::lexical_cast<float>(res[3]));
	}

	void RegisterTexture(const SplitResult & res, TextureCoords & vec)
	{
		if (res.size() != 3)
		{
			throw std::runtime_error("Unexpected coordinates number for texture");
		}
			
		vec.emplace_back(
			boost::lexical_cast<float>(res[1]),
			boost::lexical_cast<float>(res[2]));
	}

	void RegisterNormale(const SplitResult & res, PointVector & vec)
	{
		if (res.size() != 4)
		{
			throw std::runtime_error("Unexpected coordinates number for normale");
		}

		vec.emplace_back(
			boost::lexical_cast<float>(res[1]),
			boost::lexical_cast<float>(res[2]),
			boost::lexical_cast<float>(res[3]));
	}

	void RegisterGroup(const SplitResult & res, 
		const PointVector & points, const TextureCoords & textures, const NormaleVector & normales,
		IdsMap & idsMap, ModelData & md)
	{
		if (res.size() != 3)
		{
			throw std::runtime_error("Unexpected parts number for group");
		}

		if (res[0].empty() || res[1].empty() || res[2].empty())
		{
			throw std::runtime_error("A part of group is missing. All types 'vertex', 'texture', 'normale' are required");
		}

		const Ids ids = std::make_tuple(
			boost::lexical_cast<int>(res[0]) - 1,
			boost::lexical_cast<int>(res[1]) - 1,
			boost::lexical_cast<int>(res[2]) - 1);

		auto found = idsMap.find(ids);

		if (found == idsMap.end())
		{
			found = idsMap.emplace(ids, md.points.size()).first;

			md.points.push_back(points[std::get<Point>(ids)]);
			md.textures.push_back(textures[std::get<Texture>(ids)]);
			md.normales.push_back(normales[std::get<Normale>(ids)]);
		}

		md.indexes.push_back(found->second);
	}
}

ModelData
ModelReader::Read(const std::string & path)
{
	std::ifstream file(path);

	if (!file.good())
	{
		throw std::runtime_error((boost::format("Failed to read file %s") % path).str());
	}

	PointVector points;
	NormaleVector normales;
	TextureCoords textures;
	IdsMap idsMap;

	ModelData md;

	std::string line;
	while (std::getline(file, line))
	{
		SplitResult res;

		boost::split(res, line, boost::is_any_of(" "));
	
		if (res.empty())
		{
			continue;
		}

		if (res[0] == "v")
		{
			RegisterVertex(res, points);
		}
		else if (res[0] == "vt")
		{
			RegisterTexture(res, textures);
		}
		else if (res[0] == "vn")
		{
			RegisterNormale(res, normales);
		}
		else if (res[0] == "f")
		{
			if (res.size() != 4)
			{
				throw std::runtime_error("Unexpected groups number for triangle");
			}

			SplitResult groupSplit;

			boost::split(groupSplit, res[1], boost::is_any_of("/"));
			RegisterGroup(groupSplit, points, textures, normales, idsMap, md);
			groupSplit.clear();

			boost::split(groupSplit, res[2], boost::is_any_of("/"));
			RegisterGroup(groupSplit, points, textures, normales, idsMap, md);
			groupSplit.clear();

			boost::split(groupSplit, res[3], boost::is_any_of("/"));
			RegisterGroup(groupSplit, points, textures, normales, idsMap, md);
		}
	}

	return md;
}