#pragma once

#include "ModelDrawer.h"
#include "Matrix.h"

namespace trm
{
	class DrawableItem
	{
	public:
		DrawableItem(const ModelData & md, Matrix && m);

		void Draw() const;

		const Matrix & GetModelMatrix() const;

	private:
		ModelDrawer drawer_;
		Matrix modelMatrix_;
	};

} // namespace trm
