#ifndef _SHADERLOADER_H_
#define _SHADERLOADER_H_

#include <GLES2/gl2.h>

namespace trm
{
	struct Shader
	{
		enum Type
		{
			Point3d,
			Fragment
		};
	};

	struct ShaderLoader
	{
		static GLuint Load(Shader::Type t);
		static void Delete(GLuint id);
	};

} // namespace trm

#endif // _SHADERLOADER_H_