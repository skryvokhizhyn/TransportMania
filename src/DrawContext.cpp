#include "Point3d.h"
#include "DrawContext.h"
#include "MatrixUtils.h"
#include "Types.h"
#include "CommonUtils.h"
#include "ShaderLoader.h"
#include "GlobalDefines.h"

#include <GLES2/gl2.h>

using namespace trm;

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

	terrainProgram_.Init(ShaderProgramType::Terrain);
	windowProgram_.Init(ShaderProgramType::Window);

	// Sets the clear color
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
}

void DrawContext::Term()
{
	terrainProgram_.Term();
	windowProgram_.Term();
}

void DrawContext::Activate(ShaderProgramType type)
{
	switch (type)
	{
	case ShaderProgramType::Terrain:
		activeProgram_ = &terrainProgram_;
		break;

	case ShaderProgramType::Window:
		activeProgram_ = &windowProgram_;
		break;

	default:
		throw std::runtime_error(
			(boost::format("Trying to activate unknown shader program type %d") % static_cast<int>(type)).str());
	}

	// Actually use the created program
	glUseProgram(activeProgram_->Id());
}

void DrawContext::Transform(const Matrix & pv, const Matrix & m) const
{
	glUniformMatrix4fv(activeProgram_->PVLocation(), 1, GL_FALSE, pv.data());
	glUniformMatrix4fv(activeProgram_->MVLocation(), 1, GL_FALSE, m.data());
}

void trm::DrawContext::SetLightPosition(const Point3d & p) const
{
	glUniform3f(activeProgram_->LightPosition(), p.x(), p.y(), p.z());
}

void DrawContext::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
