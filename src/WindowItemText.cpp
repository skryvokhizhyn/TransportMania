#include "WindowItemText.h"
#include "WindowItemRenderer.h"

using namespace trm;

WindowItemText::WindowItemText(const std::wstring & text, float size, WindowPosition horPos, WindowPosition verPos)
	: WindowItem(UniqueId(), TextureId::Font, EventHandlerPtr(), horPos, verPos)
	, text_(text)
	, size_(size)
{}

void 
WindowItemText::Accept(WindowItemRenderer & renderer)
{
	renderer.Visit(*this);
}

const std::wstring &
WindowItemText::GetText() const
{
	return text_;
}

float
WindowItemText::GetSize() const
{
	return size_;
}