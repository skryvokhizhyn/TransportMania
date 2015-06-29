#pragma once

#include "Point3d.h"
#include "OptionalPoint3d.h"

namespace trm
{
	struct TangentPoint
	{
		Point3d point;
		OptionalPoint3d direction;
	};

} // namespace trm
