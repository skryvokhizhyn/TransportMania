#pragma once

namespace trm
{
	struct ModelData;
	class RailRoadArc;

namespace visitor_impl
{
	struct RailRoadRendererArc
	{
		static void Do(const RailRoadArc & rra, ModelData & md);
	};

} // namespace visitor_impl
} // namespace trm