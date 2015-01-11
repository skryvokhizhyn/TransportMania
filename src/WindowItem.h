#pragma once 

#include "WindowPosition.h"
#include "EventHandler.h"
#include "TextureId.h"
#include "UniqueId.h"

#include <memory>

namespace trm
{
	class WindowItem;
	class WindowItemRenderer;

	using WindowItemPtr = std::unique_ptr<WindowItem>;

	class WindowItem
		: boost::noncopyable
	{
	public:
		virtual ~WindowItem() {}

		virtual void Accept(WindowItemRenderer &) = 0;

		UniqueId GetId() const;
		TextureId GetTextureId() const;
		const EventHandlerPtr & GetHandler() const;
		WindowPosition GetHorizontalPosition() const;
		WindowPosition GetVerticalPosition() const;

	protected:
		WindowItem(UniqueId id, TextureId textureId, EventHandlerPtr handlerPtr, WindowPosition horPos, WindowPosition verPos);

	private:
		const UniqueId id_;
		const TextureId textureId_;
		const EventHandlerPtr handlerPtr_;
		const WindowPosition horizontalPosition_;
		const WindowPosition verticalPosition_;
	};

} // namespace trm
