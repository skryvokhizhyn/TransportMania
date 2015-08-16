#pragma once

#include "RailRoadVisitor.h"
#include "Polygon3d.h"
#include "RailRoad.h"

#include <utility>
#include <vector>

namespace trm
{
	using RailRoadSplitResult = std::vector<RailRoadPtr>;

	class RailRoadSplitter
		: public RailRoadVisitor
	{
	public:
		RailRoadSplitter(const Polygon3d & p);

		virtual void Visit(RailRoadArc & rra) override;
		virtual void Visit(RailRoadLine & rrl) override;

		const RailRoadSplitResult & GetSplitResult() const;

	private:
		Polygon3d splitPoints_;
		const RailRoadPtr originalRoadPtr_;
		RailRoadSplitResult spitResult_;
	};

} // namespace trm
