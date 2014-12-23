#include "TextManager.h"

using namespace trm;

TextManager::TextManager(const ModelData & md, Matrix && m)
	: modelMatrix_(std::move(m))
{
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
