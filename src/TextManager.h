#pragma once

#include "ModelDrawer.h"
#include "Matrix.h"

#include <string>

namespace trm
{
	class TextManager
	{
	public:
		TextManager(std::wstring str);

		void Draw() const;

		const Matrix & GetModelMatrix() const;

	private:
		ModelDrawer drawer_;
		Matrix modelMatrix_;
	};

} // namespace trm
