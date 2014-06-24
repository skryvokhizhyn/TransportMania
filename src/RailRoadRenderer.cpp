#include "RailRoadRenderer.h"
#include "RailRoadArc.h"
#include "RailRoadLine.h"
#include "RailRoadRendererLine.h"
#include "RailRoadRendererArc.h"

using namespace trm;

RailRoadRenderer::RailRoadRenderer(ModelData & md)
	: md_(md)
{
}

void 
RailRoadRenderer::Visit(RailRoadLine & rrl)
{
	visitor_impl::RailRoadRendererLine::Do(rrl, md_);
}

void 
RailRoadRenderer::Visit(RailRoadArc & rra)
{
	visitor_impl::RailRoadRendererArc::Do(rra, md_);
}