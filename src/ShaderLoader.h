#ifndef _SHADERLOADER_H_
#define _SHADERLOADER_H_

#include "Types.h"

namespace trm
{
	enum class ShaderType
	{
		TerrainPoint,
		TerrainFragment,
		WindowPoint,
		WindowFragment
	};

	struct ShaderLoader
	{
		static GLuintType Load(ShaderType t);
		static void Delete(GLuintType id);
	};

} // namespace trm

#endif // _SHADERLOADER_H_