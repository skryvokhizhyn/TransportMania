#pragma once

#include "RailRoadVisitor.h"

namespace trm
{
	class RailRoadSizer
		: public RailRoadVisitor
	{
	public:
		RailRoadSizer();

		virtual void Visit(RailRoadArc & rra) override;
		virtual void Visit(RailRoadLine & rrl) override;

		float GetLenght() const;

	private:
		float length_;
	};

} // namespace trm
