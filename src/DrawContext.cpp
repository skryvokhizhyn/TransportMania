#include "Point3d.h"
#include "DrawContext.h"
#include "MatrixUtils.h"
#include "Types.h"
#include "CommonUtils.h"
#include "ShaderLoader.h"
#include "GlobalDefines.h"

using namespace trm;

namespace
{
	const char * PV_MATRIX_SHADER_VARIABLE = "u_myPVMatrix";
	const char * Mv_MATRIX_SHADER_VARIABLE = "u_myMvMatrix";
}

void DrawContext::Init()
{
#ifdef DRAWING_MODE_FULL
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glCullFace(GL_BACK);
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif // DRAWING_MODE_FULL

	m_uiVertexShader = ShaderLoader::Load(Shader::Point3d);
	m_uiFragShader = ShaderLoader::Load(Shader::Fragment);

	// Create the shader program
    m_uiProgramObject = glCreateProgram();

	// Attach the fragment and vertex shaders to it
    glAttachShader(m_uiProgramObject, m_uiFragShader);
    glAttachShader(m_uiProgramObject, m_uiVertexShader);

	// Bind the custom vertex attribute "a_vertex" to location VERTEX_ARRAY
    glBindAttribLocation(m_uiProgramObject, VertexArray, "a_vertex");
	glBindAttribLocation(m_uiProgramObject, NormalArray, "a_normal");
	glBindAttribLocation(m_uiProgramObject, TextureArray, "a_texture");

	// Link the program
    glLinkProgram(m_uiProgramObject);

	// Check if linking succeeded in the same way we checked for compilation success
    GLint bLinked;
    glGetProgramiv(m_uiProgramObject, GL_LINK_STATUS, &bLinked);
	if (!bLinked)
	{
		throw std::runtime_error("Failed to link program.");
	}

	// Actually use the created program
	glUseProgram(m_uiProgramObject);

	// Sets the clear color
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
}

void DrawContext::Term()
{
	// Frees the OpenGL handles for the program and the 2 shaders
	glDeleteProgram(m_uiProgramObject);

	ShaderLoader::Delete(m_uiFragShader);
	ShaderLoader::Delete(m_uiVertexShader);
}

void DrawContext::Transform(const Matrix & pv, const Matrix & m) const
{
	// First gets the location of that variable in the shader using its name
	int varLocation = glGetUniformLocation(m_uiProgramObject, PV_MATRIX_SHADER_VARIABLE);

	// Then passes the matrix to that variable
	// data of matrix is transponated because OGL expects it in column_major way but we have row_major
	// thus in shaders we do vec * matrix instead of matrix * vec
	glUniformMatrix4fv(varLocation, 1, GL_FALSE, pv.data());

	varLocation = glGetUniformLocation(m_uiProgramObject, Mv_MATRIX_SHADER_VARIABLE);
	glUniformMatrix4fv(varLocation, 1, GL_FALSE, m.data());
}

void DrawContext::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
