#include "ShaderLoader.h"
#include "GlobalDefines.h"
#include <stdexcept>

using namespace trm;

namespace
{

#ifdef DRAWING_MODE_FULL
		const std::string VERTEX_SHADER = "\
		attribute vec3 a_vertex;\
		attribute vec3 a_normal;\
		attribute vec2 a_texture;\
		uniform mediump mat4 u_myPVMatrix;\
		uniform mediump mat4 u_myMvMatrix;\
		varying vec3 v_vertex;\
		varying vec3 v_normal;\
		varying vec2 v_texture;\
		void main(void)\
		{\
			v_vertex = a_vertex;\
			vec3 n_normal = normalize(a_normal);\
			v_normal = n_normal;\
			v_texture = a_texture;\
			gl_Position = vec4(a_vertex, 1.0) * (u_myMvMatrix * u_myPVMatrix);\
			gl_PointSize = 2.0;\
		}";

	const std::string FRAGMENT_SHADER = "\
		precision mediump float;\
		varying vec3 v_vertex;\
		varying vec3 v_normal;\
		varying vec2 v_texture;\
		uniform sampler2D u_texture;\
		void main(void)\
		{\
			vec3 u_lightPosition = vec3(0.0, 0.0, 100.0);\
			vec3 n_normal = normalize(v_normal);\
			vec3 lightVector = normalize(u_lightPosition - v_vertex);\
			float diffuse = 0.8 * max(dot(n_normal, lightVector), 0.0);\
			vec4 one = vec4(1.0, 1.0, 0.66, 1.0);\
			gl_FragColor = (0.2 + diffuse) * one * texture2D(u_texture, v_texture);\
		}";
#else

	const std::string VERTEX_SHADER = "\
		attribute vec3	a_vertex;\
		uniform mediump mat4 u_myPVMatrix;\
		uniform mediump mat4 u_myMvMatrix;\
		void main(void)\
		{\
			gl_Position = vec4(a_vertex, 1.0) * (u_myMvMatrix * u_myPVMatrix);\
			gl_PointSize = 2.0;\
		}";

	const std::string FRAGMENT_SHADER = "\
		void main(void)\
		{\
			gl_FragColor = vec4(1.0, 1.0, 0.66, 1.0);\
		}";
#endif // DRAWING_MODE_FULL

	GLuint LoadShader(const std::string & code, GLuint type)
	{
		// Create shader object
		const GLuint shaderId = glCreateShader(type);

		const char * c_str = code.c_str();

		// Load the source code into it
		glShaderSource(shaderId, 1, (const char**)&c_str, 0);

		// Compile the source code
		glCompileShader(shaderId);

		// Check if compilation succeeded
		GLint bShaderCompiled;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &bShaderCompiled);
		if (!bShaderCompiled)
		{
			throw std::runtime_error("Failed to compile fragment shader.");
		}

		return shaderId;
	}
}

GLuint ShaderLoader::Load(Shader::Type type)
{
	switch (type)
	{
	case Shader::Point3d:
		return LoadShader(VERTEX_SHADER, GL_VERTEX_SHADER);
	case Shader::Fragment:
		return LoadShader(FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
	default:
		throw std::runtime_error("Not implemented shader type");
	};
}

void ShaderLoader::Delete(GLuint id)
{
	glDeleteShader(id);
}