#include "WindowImpl.h"
#include "DrawContext.h"
#include "CachedHandlerLocatorProxy.h"

#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/adaptor/filtered.hpp>

using namespace trm;

WindowImpl::WindowImpl(const WindowItemRenderer::RenderedData & renderedData)
{
	data_.reserve(renderedData.size());

	boost::transform(renderedData, std::back_inserter(data_),
		[](const WindowItemRenderer::Data & d) -> WindowData
	{
		if (d.handlerPtr)
		{
			CachedHandlerLocatorProxy()->Put(d.id, d.polygon, HandlerLevel::Button, d.handlerPtr);
			return WindowData(d.id, d.drawableItem, true);
		}
		else
		{
			return WindowData(d.id, d.drawableItem, false);
		}
	});
}

UniqueId
WindowImpl::Id() const
{
	// first data item represents top level window
	return data_.at(0).id;
}

void
WindowImpl::Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const
{
	boost::for_each(data_, 
		[&](const WindowData & d)
	{
		const auto & drawable = d.drawableItem;

		c.Transform(orthoViewMatrix, drawable.GetModelMatrix());
		drawable.Draw();
	});
}

void
WindowImpl::Close()
{
	boost::for_each(data_ | boost::adaptors::filtered(boost::bind(&WindowData::hasHandler, _1)), 
		[](const WindowData & data)
	{
		CachedHandlerLocatorProxy()->Remove(data.id);
	});
}
