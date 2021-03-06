#pragma once

#include <string>

namespace trm
{
	std::string GetDataFolderPath();
	std::string GetRelativePath(const std::initializer_list<std::string> & il);

	std::string GetHeightMapPath(const std::string & name);
	std::string GetFontPath(const std::string & name);
	std::string GetTexturePath(const std::string & name);
	std::string GetModelPath(const std::string & name);

} // namespace trm
