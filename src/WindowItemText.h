#pragma once

#include "WindowItem.h"

#include <string>

namespace trm
{
	class WindowItemText
		: public WindowItem
	{
	public:
		WindowItemText(const std::wstring & text, float size, WindowPosition horPos, WindowPosition verPos);

		virtual void Accept(WindowItemRenderer & renderer) override;

		const std::wstring & GetText() const;
		float GetSize() const;

	private:
		const std::wstring text_;
		const float size_;
	};

} // namespace trm
