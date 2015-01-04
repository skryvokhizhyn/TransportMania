#include "WindowManager.h"
#include "DrawContext.h"
#include "WindowRenderer.h"
#include "ModelData.h"
#include "MatrixFactory.h"
#include "EventHandlerLocatorProxy.h"
#include "EventHandlerWrapper.h"

#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/adaptor/map.hpp>

using namespace trm;

namespace
{
	const float OK_WINDOW_RELATIVE_SIZE = 0.2f;
}

void
WindowManager::Init(const size_t width, const size_t height)
{
	width_ = width;
	height_ = height;
}

void
WindowManager::CreateOKWindow(OnEventCallback cb)
{
	const float okWindowSize = OK_WINDOW_RELATIVE_SIZE * height_;
	const Size2d windowSize(
		boost::numeric_cast<size_t>(okWindowSize),
		boost::numeric_cast<size_t>(okWindowSize));

	ModelData md = WindowRenderer::RenderRectangleWindow(windowSize, TextureId::OkButton);

	const float xPos = width_ / 2.0f - okWindowSize / 2;
	const float yPos = height_ / 2.0f - okWindowSize / 2;

	Matrix pos = MatrixFactory::Move(Point3d(xPos, yPos, 0.0f));

	const int id = windows_.size();

	Polygon2d polygon;
	boost::transform(md.points, std::back_inserter(polygon),
		[&](const Point3d & p)
	{
		return Point2d::Cast(p) + Point2d(xPos, yPos);
	});

	windows_.emplace(id, DrawableItem(md, std::move(pos)));
	EventHandlerLocatorProxy()->Put(id, polygon, std::make_shared<EventHandlerWrapper>(cb));
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
