#ifndef _DRAWCONTEXT_H_
#define _DRAWCONTEXT_H_

#include "Matrix.h"
#include "ShaderProgram.h"

namespace trm
{
	class DrawContext
	{
	public:
		void Init();
		void Term();
		void Clear() const;
		
		void Activate(ShaderProgramType type);

		void Transform(const Matrix & pv, const Matrix & m) const;

	private:
		ShaderProgram terrainProgram_;
		ShaderProgram windowProgram_;
		ShaderProgram * activeProgram_ = nullptr;
	};

} // namespace trm

#endif // _DRAWCONTEXT_H_