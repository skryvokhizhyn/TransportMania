#include "LocomotiveDrawableObject.h"
#include "ModelData.h"
#include "GlobalDefines.h"

#pragma warning(push)
#pragma warning(disable: 4512)

#include <boost/assign/std/vector.hpp>

#pragma warning(pop)

using namespace trm;
using namespace boost::assign;

namespace 
{
	void RenderLines(ModelData & md)
	{
		md.indexes += 0, 1, 1, 2, 2, 3, 3, 4, 4, 0; // lower
		md.indexes += 5, 6, 6, 7, 7, 8, 8, 9, 9, 5; // upper
		md.indexes += 0, 5, 1, 6, 2, 7, 3, 8, 4, 9; // connections
	}

	void RenderTriangles(ModelData & md)
	{
		md.points.push_back(Point3d(-0.7f, -0.7f, 0.0f)); // 10
		md.points.push_back(Point3d(-0.7f, -0.7f, 0.7f));
		md.points.push_back(Point3d(0.7f, -0.7f, 0.0f)); // 12
		md.points.push_back(Point3d(0.7f, -0.7f, 0.7f));

		md.points.push_back(Point3d(0.7f, -0.7f, 0.0f)); // 14
		md.points.push_back(Point3d(0.7f, -0.7f, 0.7f));
		md.points.push_back(Point3d(1.5f, 0.0f, 0.0f)); // 16
		md.points.push_back(Point3d(1.5f, 0.0f, 0.7f));

		md.points.push_back(Point3d(1.5f, 0.0f, 0.0f)); // 18
		md.points.push_back(Point3d(1.5f, 0.0f, 0.7f));
		md.points.push_back(Point3d(0.7f, 0.7f, 0.0f)); // 20
		md.points.push_back(Point3d(0.7f, 0.7f, 0.7f));

		md.points.push_back(Point3d(0.7f, 0.7f, 0.0f)); // 22
		md.points.push_back(Point3d(0.7f, 0.7f, 0.7f));
		md.points.push_back(Point3d(-0.7f, 0.7f, 0.0f)); // 24
		md.points.push_back(Point3d(-0.7f, 0.7f, 0.7f));

		md.points.push_back(Point3d(-0.7f, 0.7f, 0.0f)); // 26
		md.points.push_back(Point3d(-0.7f, 0.7f, 0.7f));
		md.points.push_back(Point3d(-0.7f, -0.7f, 0.0f)); // 28
		md.points.push_back(Point3d(-0.7f, -0.7f, 0.7f));

		md.indexes += 1, 0, 2, 2, 0, 3, 3, 0, 4; // lower
		md.indexes += 6, 7, 5, 5, 7, 8, 5, 8, 9; // upper

		md.indexes += 10, 12, 11, 11, 12, 13;
		md.indexes += 14, 16, 15, 15, 16, 17;
		md.indexes += 18, 20, 19, 19, 20, 21;
		md.indexes += 22, 24, 23, 23, 24, 25;
		md.indexes += 26, 28, 27, 27, 28, 29;

		md.normales += // lower
			Point3d(0.0f, 0.0f, -1.0f), 
			Point3d(0.0f, 0.0f, -1.0f),
			Point3d(0.0f, 0.0f, -1.0f),
			Point3d(0.0f, 0.0f, -1.0f),
			Point3d(0.0f, 0.0f, -1.0f);

		md.normales += // upper
			Point3d(0.0f, 0.0f, 1.0f), 
			Point3d(0.0f, 0.0f, 1.0f),
			Point3d(0.0f, 0.0f, 1.0f),
			Point3d(0.0f, 0.0f, 1.0f),
			Point3d(0.0f, 0.0f, 1.0f);

		md.normales += // 
			Point3d(0.0f, -1.0f, 1.0f),
			Point3d(0.0f, -1.0f, 1.0f),
			Point3d(0.0f, -1.0f, 1.0f),
			Point3d(0.0f, -1.0f, 1.0f);

		md.normales += //
			Point3d(1.0f, -1.0f, 1.0f),
			Point3d(1.0f, -1.0f, 1.0f),
			Point3d(1.0f, -1.0f, 1.0f),
			Point3d(1.0f, -1.0f, 1.0f);

		md.normales += //
			Point3d(1.0f, -1.0f, 1.0f),
			Point3d(1.0f, -1.0f, 1.0f),
			Point3d(1.0f, -1.0f, 1.0f),
			Point3d(1.0f, -1.0f, 1.0f);

		md.normales += //
			Point3d(0.0f, 1.0f, 1.0f),
			Point3d(0.0f, 1.0f, 1.0f),
			Point3d(0.0f, 1.0f, 1.0f),
			Point3d(0.0f, 1.0f, 1.0f);

		md.normales += //
			Point3d(-1.0f, 0.0f, 1.0f),
			Point3d(-1.0f, 0.0f, 1.0f),
			Point3d(-1.0f, 0.0f, 1.0f),
			Point3d(-1.0f, 0.0f, 1.0f);
	}
}

void
LocomotiveDrawableObject::Render(ModelData & md)
{
	// lower surface
	md.points.push_back(Point3d(-0.7f, -0.7f, 0.0f));
	md.points.push_back(Point3d(0.7f, -0.7f, 0.0f));
	md.points.push_back(Point3d(1.5f, 0.0f, 0.0f));
	md.points.push_back(Point3d(0.7f, 0.7f, 0.0f));
	md.points.push_back(Point3d(-0.7f, 0.7f, 0.0f));

	// upper surface
	md.points.push_back(Point3d(-0.7f, -0.7f, 0.7f));
	md.points.push_back(Point3d(0.7f, -0.7f, 0.7f));
	md.points.push_back(Point3d(1.5f, 0.0f, 0.7f));
	md.points.push_back(Point3d(0.7f, 0.7f, 0.7f));
	md.points.push_back(Point3d(-0.7f, 0.7f, 0.7f));

#ifdef DRAWING_MODE_FULL
	RenderTriangles(md);

	md.type = ModelData::Mode::Triangle;
#else
	RenderLines(md);

	md.type = ModelData::Mode::Line;
#endif // DRAWING_MODE_FULL

	std::transform(md.points.cbegin(), md.points.cend(), md.points.begin(),
		[](decltype(*md.points.cbegin()) & p)
	{
		return p + Point3d(0.7f, 0.0f, 0.0f);
	});

	CalculateLength(md);
}
