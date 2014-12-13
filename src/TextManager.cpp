#include "TextManager.h"
#include "TextRenderer.h"
#include "ModelData.h"
#include "MatrixFactory.h"
#include "MatrixUtils.h"
#include "GlobalDefines.h"
#include "FontReader.h"
#include "Settings.h"

using namespace trm;

TextManager::TextManager(const FontData & fd, std::wstring str)
	: modelMatrix_(MatrixFactory::Move(Point3d(-1.0f, 0.9f, 0.0f)) * MatrixFactory::Scale(0.05f / 20))
{
#ifndef DRAWING_MODE_FULL
	ModelData md = TextRenderer::Render(str);
#else
	ModelData md = TextRenderer::Render(fd, str);
#endif // DRAWING_MODE_FULL
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
