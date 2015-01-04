#include "ShaderProgram.h"
#include "ShaderLoader.h"
#include "ShaderAttribute.h"

#include <GLES2/gl2.h>

using namespace trm;

namespace
{
	const GLuintType UNDEFINED_ID = std::numeric_limits<GLuintType>::max();
	const char * PV_MATRIX_SHADER_VARIABLE = "u_myPVMatrix";
	const char * Mv_MATRIX_SHADER_VARIABLE = "u_myMvMatrix";
}

ShaderProgram::ShaderProgram()
	: m_uiFragShader(UNDEFINED_ID)
	, m_uiVertexShader(UNDEFINED_ID)
	, m_uiProgramObject(UNDEFINED_ID)
	, m_pvLocation(UNDEFINED_ID)
	, m_mvLocation(UNDEFINED_ID)
{}

void
ShaderProgram::Init(ShaderProgramType type)
{
	switch (type)
	{
	default:
		throw std::runtime_error((boost::format("Unknown shader program id=%d given") % static_cast<int>(type)).str());

	case ShaderProgramType::Terrain:
		m_uiVertexShader = ShaderLoader::Load(ShaderType::TerrainPoint);
		m_uiFragShader = ShaderLoader::Load(ShaderType::TerrainFragment);
		break;

	case ShaderProgramType::Window:
		m_uiVertexShader = ShaderLoader::Load(ShaderType::WindowPoint);
		m_uiFragShader = ShaderLoader::Load(ShaderType::WindowFragment);
		break;
	}

	// Create the shader program
    m_uiProgramObject = glCreateProgram();

	// Attach the fragment and vertex shaders to it
    glAttachShader(m_uiProgramObject, m_uiFragShader);
    glAttachShader(m_uiProgramObject, m_uiVertexShader);

	// Bind the custom vertex attribute "a_vertex" to location VERTEX_ARRAY
    glBindAttribLocation(m_uiProgramObject, ShaderAttribute::VertexArray, "a_vertex");
	glBindAttribLocation(m_uiProgramObject, ShaderAttribute::NormalArray, "a_normal");
	glBindAttribLocation(m_uiProgramObject, ShaderAttribute::TextureArray, "a_texture");

	// Link the program
    glLinkProgram(m_uiProgramObject);

	// Check if linking succeeded in the same way we checked for compilation success
    GLint bLinked;
    glGetProgramiv(m_uiProgramObject, GL_LINK_STATUS, &bLinked);
	if (!bLinked)
	{
		throw std::runtime_error("Failed to link program.");
	}

	// First gets the location of that variable in the shader using its name
	m_pvLocation = glGetUniformLocation(m_uiProgramObject, PV_MATRIX_SHADER_VARIABLE);
	m_mvLocation = glGetUniformLocation(m_uiProgramObject, Mv_MATRIX_SHADER_VARIABLE);
}

void
ShaderProgram::Term()
{
	// Frees the OpenGL handles for the program and the 2 shaders
	glDeleteProgram(m_uiProgramObject);

	ShaderLoader::Delete(m_uiFragShader);
	ShaderLoader::Delete(m_uiVertexShader);
}

GLuintType 
ShaderProgram::Id() const
{
	return m_uiProgramObject;
}

GLuintType 
ShaderProgram::PVLocation() const
{
	return m_pvLocation;
}

GLuintType 
ShaderProgram::MVLocation() const
{
	return m_mvLocation;
}