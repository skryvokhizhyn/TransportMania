#include "DrawableItem.h"

using namespace trm;

DrawableItem::DrawableItem(const ModelData & md, Matrix && m)
	: modelMatrix_(std::move(m))
{
	drawer_.Load(md);
}

void
DrawableItem::Draw() const
{
	drawer_.Draw();
}

const Matrix & 
DrawableItem::GetModelMatrix() const
{
	return modelMatrix_;
}
