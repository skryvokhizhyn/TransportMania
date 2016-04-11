#pragma once

#include "Point3d.h"
#include "Point2d.h"
#include "Angle.h"
#include "Line.h"

#include <boost/optional.hpp>

namespace trm
{
	class LinearHeightGetter
	{
	public:
		LinearHeightGetter(Point3d p1, Point3d p2);

		float operator() (Point2d p) const;

	private:
		using OptionalLine = boost::optional<Line>;

	private:
		float initialHeight_;
		Angle angle_;
		OptionalLine lNorm_;
	};

} // namespace trm

