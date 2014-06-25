#pragma once

#include <string>

namespace trm
{
	std::string GetDataFolderPath();

	// utils
	std::string GetRelativePath(const std::initializer_list<std::string> & il);

} // namespace trm
