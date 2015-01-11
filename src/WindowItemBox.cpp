#include "WindowItemBox.h"
#include "WindowItemRenderer.h"

#include <boost/range/algorithm/for_each.hpp>

using namespace trm;

WindowItemBox::WindowItemBox(UniqueId id, const Size2f & size, TextureId textureId, EventHandlerPtr handlerPtr,
	WindowPosition horPos, WindowPosition verPos)
	: WindowItem(id, textureId, handlerPtr, horPos, verPos)
	, size_(size)
{}

void 
WindowItemBox::Add(WindowItemPtr itemPtr)
{
	children_.emplace_back(std::move(itemPtr));
}

void
WindowItemBox::Accept(WindowItemRenderer & visitor)
{
	Rectangle2f subRectangle = visitor.Visit(*this);

	WindowItemRenderer subRenderer(subRectangle);

	boost::for_each(children_, boost::bind(&WindowItem::Accept, _1, boost::ref(subRenderer)));

	visitor.Accumulate(subRenderer);
}

const Size2f & 
WindowItemBox::GetSize() const
{
	return size_;
}