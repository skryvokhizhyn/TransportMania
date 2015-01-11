#pragma once

#include "WindowItem.h"
#include "Size2f.h"

namespace trm
{
	class WindowItemBox
		: public WindowItem
	{
	public:
		WindowItemBox(UniqueId id, const Size2f & size, TextureId textureId, EventHandlerPtr handlerPtr, 
			WindowPosition horPos, WindowPosition verPos);

		virtual void Accept(WindowItemRenderer & visitor) override;

		void Add(WindowItemPtr itemPtr);
		const Size2f & GetSize() const;

	private:
		using WindowItems = std::vector<WindowItemPtr>;

	private:
		WindowItems children_;
		const Size2f size_;
	};

	using WindowItemBoxPtr = std::unique_ptr<WindowItemBox>;

} // namespace trm
