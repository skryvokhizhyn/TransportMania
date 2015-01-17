#include "WindowItemRenderer.h"
#include "WindowItemBox.h"
#include "WindowItemText.h"
#include "WindowRenderer.h"
#include "ModelData.h"
#include "MatrixFactory.h"
#include "TextRendererProxy.h"

#include <boost/range/algorithm/transform.hpp>

using namespace trm;

namespace
{
	Polygon2d CreatePolygonFromPoints(const PointVector & points, const Point2d & loc)
	{
		Polygon2d polygon;
		polygon.reserve(points.size());
		boost::transform(points, std::back_inserter(polygon),
			[&](const Point3d & p)
		{
			return Point2d::Cast(p) + loc;
		});

		return polygon;
	}
}

WindowItemRenderer::Data::Data(const UniqueId & i, EventHandlerPtr h, const DrawableItem & d, Polygon2d && p)
	: id(i), handlerPtr(std::move(h)), drawableItem(d), polygon(std::move(p))
{}

WindowItemRenderer::WindowItemRenderer(const Rectangle2f & background)
	: background_(background)
{}

const WindowItemRenderer::RenderedData & 
WindowItemRenderer::GetData() const
{
	return renderedData_;
}

Rectangle2f
WindowItemRenderer::Visit(WindowItemBox & box)
{
	Size2f backgroundSize(background_.width, background_.height);
	Size2d baseSize = Size2d::Cast(backgroundSize);
	Size2d boxSize = Size2d::Cast(box.GetSize());

	ModelData md = WindowRenderer::RenderRectangleWindow(boxSize, box.GetTextureId());

	Point2d loc = GetWindowPosition(baseSize, Size2d::Cast(boxSize), box.GetHorizontalPosition(), box.GetVerticalPosition());
	loc += background_.bottomLeft;

	Matrix pos = MatrixFactory::Move(Point3d::Cast(loc));

	Polygon2d polygon = CreatePolygonFromPoints(md.points, loc);

	renderedData_.emplace_back(Data{box.GetId(), box.GetHandler(), DrawableItem(md, std::move(pos)), std::move(polygon)});

	return Rectangle2f(loc, box.GetSize().x(), box.GetSize().y());
}

void 
WindowItemRenderer::Visit(WindowItemText & text)
{
	Size2f backgroundSize(background_.width, background_.height);
	Size2d baseSize = Size2d::Cast(backgroundSize);

	TextData textData = TextRendererProxy()->Render(text.GetText(), text.GetSize(), background_.width);
	Point2d textLoc = GetWindowPosition(baseSize, Size2d::Cast(textData.size), text.GetHorizontalPosition(), text.GetVerticalPosition());
	textLoc += background_.bottomLeft;

	Matrix pos = MatrixFactory::Move(Point3d::Cast(textLoc));

	Polygon2d polygon = CreatePolygonFromPoints(textData.modelData.points, textLoc);

	renderedData_.emplace_back(Data{text.GetId(), text.GetHandler(), DrawableItem(textData.modelData, std::move(pos)), std::move(polygon)});
}

void 
WindowItemRenderer::Accumulate(WindowItemRenderer & subRenderer)
{
	renderedData_.splice(renderedData_.end(), subRenderer.renderedData_);
}
