#include "VehicleDrawableObject.h"
#include "ModelData.h"
#include <algorithm>

using namespace trm;

float 
VehicleDrawableObject::Length() const
{
	return 1.5f + 0.7f;
}

float 
VehicleDrawableObject::Shift() const
{
	return 0.7f;
}

void
VehicleDrawableObject::Render(ModelData & md)
{
	md.points.push_back(Point3d(-0.7f, -0.7f, 0.0f));
	md.points.push_back(Point3d(0.7f, -0.7f, 0.0f));
	md.points.push_back(Point3d(1.5f, 0.0f, 0.0f));
	md.points.push_back(Point3d(0.7f, 0.7f, 0.0f));
	md.points.push_back(Point3d(-0.7f, 0.7f, 0.0f));

	std::transform(md.points.begin(), md.points.end(), md.points.begin(), 
		[](decltype(*md.points.cbegin()) & p)
	{
		return p + Point3d(0.7f, 0.0f, 0.0f);
	});

	md.indexes.push_back(0);
	md.indexes.push_back(1);
	md.indexes.push_back(1);
	md.indexes.push_back(2);
	md.indexes.push_back(2);
	md.indexes.push_back(3);
	md.indexes.push_back(3);
	md.indexes.push_back(4);
	md.indexes.push_back(4);
	md.indexes.push_back(0);
}
