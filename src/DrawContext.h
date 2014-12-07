#ifndef _DRAWCONTEXT_H_
#define _DRAWCONTEXT_H_

#include "Matrix.h"

namespace trm
{
	class DrawContext
	{
	public:
		enum VariableId
		{
			VertexArray = 0,
			NormalArray,
			TextureArray
		};

	public:
		void Init();
		void Term();
		void Clear() const;
		
		void Transform(const Matrix & pv, const Matrix & m) const;

	private:
		// The vertex and fragment shader OpenGL handles
		GLuintType m_uiVertexShader, m_uiFragShader;
		// The program object containing the 2 shader objects
		GLuintType m_uiProgramObject;
	};

} // namespace trm

#endif // _DRAWCONTEXT_H_