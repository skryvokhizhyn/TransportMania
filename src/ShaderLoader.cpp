#include "ShaderLoader.h"
#include "GlobalDefines.h"

#include <GLES2/gl2.h>

#include <stdexcept>

using namespace trm;

namespace
{

#ifdef DRAWING_MODE_FULL
		const std::string TERRAIN_VERTEX_SHADER = "\
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
		}";

	const std::string TERRAIN_FRAGMENT_SHADER = "\
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
			vec3 one = vec3(1.0, 1.0, 0.66);\
			vec4 texture = texture2D(u_texture, v_texture);\
			gl_FragColor = vec4((0.2 + diffuse) * one * texture.rgb, texture.a);\
		}";

	const std::string WINDOW_VERTEX_SHADER = "\
		attribute vec3 a_vertex;\
		attribute vec2 a_texture;\
		uniform mediump mat4 u_myPVMatrix;\
		uniform mediump mat4 u_myMvMatrix;\
		varying vec3 v_vertex;\
		varying vec2 v_texture;\
		void main(void)\
		{\
			v_vertex = a_vertex;\
			v_texture = a_texture;\
			gl_Position = vec4(a_vertex, 1.0) * (u_myMvMatrix * u_myPVMatrix);\
		}";

	const std::string WINDOW_FRAGMENT_SHADER = "\
		precision mediump float;\
		varying vec3 v_vertex;\
		varying vec2 v_texture;\
		uniform sampler2D u_texture;\
		void main(void)\
		{\
			gl_FragColor = texture2D(u_texture, v_texture);;\
		}";

#else

	const std::string TERRAIN_VERTEX_SHADER = "\
		attribute vec3	a_vertex;\
		uniform mediump mat4 u_myPVMatrix;\
		uniform mediump mat4 u_myMvMatrix;\
		void main(void)\
		{\
			gl_Position = vec4(a_vertex, 1.0) * (u_myMvMatrix * u_myPVMatrix);\
			gl_PointSize = 2.0;\
		}";

	const std::string TERRAIN_FRAGMENT_SHADER = "\
		void main(void)\
		{\
			gl_FragColor = vec4(1.0, 1.0, 0.66, 1.0);\
		}";

	const std::string WINDOW_VERTEX_SHADER = TERRAIN_VERTEX_SHADER;
	const std::string WINDOW_FRAGMENT_SHADER = TERRAIN_FRAGMENT_SHADER;

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
			throw std::runtime_error("Failed to compile shader.");
		}

		return shaderId;
	}
}

GLuintType ShaderLoader::Load(ShaderType type)
{
	switch (type)
	{
	case ShaderType::TerrainPoint:
		return LoadShader(TERRAIN_VERTEX_SHADER, GL_VERTEX_SHADER);
	case ShaderType::TerrainFragment:
		return LoadShader(TERRAIN_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
	case ShaderType::WindowPoint:
		return LoadShader(WINDOW_VERTEX_SHADER, GL_VERTEX_SHADER);
	case ShaderType::WindowFragment:
		return LoadShader(WINDOW_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
	default:
		throw std::runtime_error("Not implemented shader type");
	};
}

void ShaderLoader::Delete(GLuintType id)
{
	glDeleteShader(id);
}