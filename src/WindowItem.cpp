#include "WindowItem.h"

using namespace trm;

WindowItem::WindowItem(UniqueId id, TextureId textureId, EventHandlerPtr handlerPtr, WindowPosition horPos, WindowPosition verPos)
	: id_(id)
	, textureId_(textureId)
	, handlerPtr_(std::move(handlerPtr))
	, horizontalPosition_(horPos)
	, verticalPosition_(verPos)
{}

UniqueId 
WindowItem::GetId() const
{
	return id_;
}

TextureId
WindowItem::GetTextureId() const
{
	return textureId_;
}

const EventHandlerPtr &
WindowItem::GetHandler() const
{
	return handlerPtr_;
}

WindowPosition
WindowItem::GetHorizontalPosition() const
{
	return horizontalPosition_;
}

WindowPosition
WindowItem::GetVerticalPosition() const
{
	return verticalPosition_;
}
