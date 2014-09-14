#include "TextManager.h"
#include "TextRenderer.h"
#include "ModelData.h"
#include "MatrixFactory.h"
#include "MatrixUtils.h"

using namespace trm;

TextManager::TextManager(std::wstring str)
	: modelMatrix_(MatrixFactory::Move(Point3d(-1.0f, 0.9f, 0.0f)) * MatrixFactory::Scale(0.05f))
{
	ModelData md = TextRenderer::Render(str);
	drawer_.Load(md);
}

void
TextManager::Draw() const
{
	drawer_.Draw();
}

const Matrix & 
TextManager::GetModelMatrix() const
{
	return modelMatrix_;
}
