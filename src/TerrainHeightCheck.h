#pragma once

#include "Point2d.h"
#include "Matrix.h"
#include "Line.h"
#include "TerraformFunction.h"
#include "OptionalPoint3d.h"

#include <boost/ref.hpp>

namespace trm
{
	struct Point3d;

	class TerrainHeightCheck
		: public TerraformFunction
	{
	public:
		TerrainHeightCheck(const Point2d & p, const Matrix & pv);

		bool operator () (const Point2d & p, float & height);

		OptionalPoint3d Get() const;

	private:
		boost::reference_wrapper<const Matrix> pvMatrix_;
		Point2d point_;
		OptionalPoint3d pointFound_;
	};

} // namespace trm
