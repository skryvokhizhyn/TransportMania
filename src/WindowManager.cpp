#include "WindowManager.h"
#include "DrawContext.h"
#include "WindowRenderer.h"
#include "ModelData.h"
#include "MatrixFactory.h"
#include "CachedHandlerLocatorProxy.h"
//#include "EventHandlerWrapper.h"
#include "EventContainer.h"
#include "WindowPosition.h"
#include "WindowCloseEvent.h"

#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/adaptor/map.hpp>

using namespace trm;

namespace
{
	const float OK_WINDOW_RELATIVE_SIZE = 0.2f;
}

void
WindowManager::Init(const Size2d & sz)
{
	screenSize_ = sz;
}

void
WindowManager::CreateOKWindow(EventAction cb)
{
	const size_t windowSize = boost::numeric_cast<size_t>(screenSize_.y() * OK_WINDOW_RELATIVE_SIZE);

	Size2d sz(windowSize, windowSize);
	ModelData md = WindowRenderer::RenderRectangleWindow(sz, TextureId::OkButton);

	const Point2d loc = GetWindowPosition(screenSize_, sz, WindowPosition::p50, WindowPosition::p50);
	Matrix pos = MatrixFactory::Move(Point3d::Cast(loc));

	const int id = windows_.size();

	Polygon2d polygon;
	boost::transform(md.points, std::back_inserter(polygon),
		[&](const Point3d & p)
	{
		return Point2d::Cast(p) + loc;
	});

	windows_.emplace(id, DrawableItem(md, std::move(pos)));
	CachedHandlerLocatorProxy()->Put(id, polygon, EventContainer::Create({cb, WindowCloseEvent(id)}, EventActionType::Single));
}

void 
WindowManager::CloseWindow(int id)
{
	windows_.erase(id);
	CachedHandlerLocatorProxy()->Remove(id);
}

void
WindowManager::Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const
{
	boost::for_each(windows_ | boost::adaptors::map_values, 
		[&](const DrawableItem & d)
	{
		c.Transform(orthoViewMatrix, d.GetModelMatrix());
		d.Draw();
	});
}
