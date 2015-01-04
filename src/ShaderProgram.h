#pragma once

#include "Types.h"

namespace trm
{
	enum class ShaderProgramType
	{
		Terrain,
		Window
	};

	class ShaderProgram
	{
	public:
		ShaderProgram();

		void Init(ShaderProgramType type);
		void Term();

		GLuintType Id() const;
		GLuintType PVLocation() const;
		GLuintType MVLocation() const;

	private:
		// The program object containing the 2 shader objects
		GLuintType m_uiProgramObject;
		// The vertex and fragment shader OpenGL handles
		GLuintType m_uiVertexShader;
		GLuintType m_uiFragShader;
		GLuintType m_pvLocation;
		GLuintType m_mvLocation;
	};

} // namespace trm
