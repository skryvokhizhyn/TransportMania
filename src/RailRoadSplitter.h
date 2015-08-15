#pragma once

#include "RailRoadVisitor.h"
#include "Point3d.h"
#include "RailRoad.h"

#include <boost/optional.hpp>

#include <utility>

namespace trm
{
	using RailRoadSplitResult = boost::optional<std::pair<RailRoadPtr, RailRoadPtr>>;

	class RailRoadSplitter
		: public RailRoadVisitor
	{
	public:
		RailRoadSplitter(const Point3d & p);

		virtual void Visit(RailRoadArc & rra) override;
		virtual void Visit(RailRoadLine & rrl) override;

		const RailRoadSplitResult & GetSplitResult() const;

	private:
		const Point3d splitPoint_;
		const RailRoadPtr originalRoadPtr_;
		RailRoadSplitResult spitResult_;
	};

} // namespace trm
