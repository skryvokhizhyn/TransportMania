#pragma once

#include "WindowData.h"
#include "WindowItemRenderer.h"

#include <vector>

namespace trm
{
	class DrawContext;

	class WindowImpl
	{
	public:
		WindowImpl(const WindowItemRenderer::RenderedData & renderedData);

		UniqueId Id() const;

		void Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const;

		void Close();

	private:
		using Data = std::vector<WindowData>;

	private:
		Data data_;
	};

} // namespace trm
