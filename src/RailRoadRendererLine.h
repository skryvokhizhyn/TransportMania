#pragma once

namespace trm
{
	struct ModelData;
	class RailRoadLine;

namespace visitor_impl
{
	struct RailRoadRendererLine
	{
		static void Do(const RailRoadLine & rrl, ModelData & md);
	};
	
} // namespace visitor_impl
} // namespace trm