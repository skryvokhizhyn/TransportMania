#pragma once

#include "ModelDrawer.h"
#include "Matrix.h"

#include <string>

namespace trm
{
	struct FontData;

	class TextManager
	{
	public:
		TextManager(const FontData & fd, std::wstring str);

		void Draw() const;

		const Matrix & GetModelMatrix() const;

	private:
		ModelDrawer drawer_;
		Matrix modelMatrix_;
	};

} // namespace trm
