#pragma once

#include "Point2d.h"
#include "Matrix.h"
#include "Line.h"
#include "TerraformFunction.h"

#include <boost/ref.hpp>
#include <boost/optional.hpp>

namespace trm
{
	struct Point3d;

	class TerrainHeightCheck
		: public TerraformFunction
	{
	public:
		using OptionalPoint = boost::optional<Point3d>;

	public:
		TerrainHeightCheck(const Point2d & p, const Matrix & pv);

		bool operator () (const Point2d & p, float & height);

		OptionalPoint Get() const;

	private:
		boost::reference_wrapper<const Matrix> pvMatrix_;
		Point2d point_;
		OptionalPoint pointFound_;

	public:
		float dist_ = 10000.0f;
	};

} // namespace trm
