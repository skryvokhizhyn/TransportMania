#pragma once

#include "Rectangle2f.h"
#include "DrawableItem.h"
#include "EventHandler.h"
#include "Polygon2d.h"
#include "UniqueId.h"

#include <list>

namespace trm
{
	class WindowItemBox;
	class WindowItemText;

	class WindowItemRenderer
		: boost::noncopyable
	{
	public:
		struct Data
		{
			UniqueId id;
			EventHandlerPtr handlerPtr;
			DrawableItem drawableItem;
			Polygon2d polygon;

			Data(const UniqueId & i, EventHandlerPtr h, const DrawableItem & d, Polygon2d && p);
		};

		using RenderedData = std::list<Data>;

	public:
		WindowItemRenderer(const Rectangle2f & background);

		Rectangle2f Visit(WindowItemBox & box);
		void Visit(WindowItemText & text);

		const RenderedData & GetData() const;

		void Accumulate(WindowItemRenderer & subRenderer);

	private:
		const Rectangle2f background_;
		RenderedData renderedData_;
	};

} // namespace trm
