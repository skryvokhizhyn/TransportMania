#pragma once

#include "ModelData.h"

namespace trm
{
	struct ModelReader
	{
		static ModelData Read(const std::string & file);
	};

} // namespace trm
