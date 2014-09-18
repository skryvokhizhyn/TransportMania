#include "WagonDrawableObject.h"
#include "ModelData.h"

using namespace trm;

void
WagonDrawableObject::Render(ModelData & md)
{
	md.points.push_back(Point3d(-0.7f, -0.7f, 0.0f));
	md.points.push_back(Point3d(0.7f, -0.7f, 0.0f));
	md.points.push_back(Point3d(0.7f, 0.7f, 0.0f));
	md.points.push_back(Point3d(-0.7f, 0.7f, 0.0f));

	md.points.push_back(Point3d(-0.7f, -0.7f, 0.7f));
	md.points.push_back(Point3d(0.7f, -0.7f, 0.7f));
	md.points.push_back(Point3d(0.7f, 0.7f, 0.7f));
	md.points.push_back(Point3d(-0.7f, 0.7f, 0.7f));

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
	md.indexes.push_back(0);

	md.indexes.push_back(4);
	md.indexes.push_back(5);
	md.indexes.push_back(5);
	md.indexes.push_back(6);
	md.indexes.push_back(6);
	md.indexes.push_back(7);
	md.indexes.push_back(7);
	md.indexes.push_back(0);

	md.indexes.push_back(0);
	md.indexes.push_back(4);
	md.indexes.push_back(1);
	md.indexes.push_back(5);
	md.indexes.push_back(2);
	md.indexes.push_back(6);
	md.indexes.push_back(3);
	md.indexes.push_back(7);

	CalculateLength(md);
}
