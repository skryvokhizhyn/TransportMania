#include "WagonDrawableObject.h"
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
		md.indexes += 0, 1, 1, 2, 2, 3, 3, 0;
		md.indexes += 4, 5, 5, 6, 6, 7, 7, 0;
		md.indexes += 0, 4, 1, 5, 2, 6, 3, 7;
	}

	void RenderTriangles(ModelData & md)
	{
		md.points.push_back(Point3d(-0.7f, -0.7f, 0.0f)); // 8
		md.points.push_back(Point3d(-0.7f, -0.7f, 0.7f));
		md.points.push_back(Point3d(0.7f, -0.7f, 0.0f)); // 10
		md.points.push_back(Point3d(0.7f, -0.7f, 0.7f));

		md.points.push_back(Point3d(0.7f, -0.7f, 0.0f)); // 12
		md.points.push_back(Point3d(0.7f, -0.7f, 0.7f));
		md.points.push_back(Point3d(0.7f, 0.7f, 0.0f)); // 14
		md.points.push_back(Point3d(0.7f, 0.7f, 0.7f));

		md.points.push_back(Point3d(0.7f, 0.7f, 0.0f)); // 16
		md.points.push_back(Point3d(0.7f, 0.7f, 0.7f));
		md.points.push_back(Point3d(-0.7f, 0.7f, 0.0f)); // 18
		md.points.push_back(Point3d(-0.7f, 0.7f, 0.7f));

		md.points.push_back(Point3d(-0.7f, 0.7f, 0.0f)); // 20
		md.points.push_back(Point3d(-0.7f, 0.7f, 0.7f));
		md.points.push_back(Point3d(-0.7f, -0.7f, 0.0f)); // 22
		md.points.push_back(Point3d(-0.7f, -0.7f, 0.7f));

		md.indexes +=
			2, 1, 0, 2, 0, 3,
			4, 5, 6, 4, 6, 7;

		md.indexes +=
			8, 10, 9, 9, 10, 11,
			12, 14, 13, 13, 14, 15,
			16, 18, 17, 17, 18, 19,
			20, 22, 21, 21, 22, 23;

		md.normales += // lower
			Point3d(0.0f, 0.0f, -1.0f), 
			Point3d(0.0f, 0.0f, -1.0f),
			Point3d(0.0f, 0.0f, -1.0f),
			Point3d(0.0f, 0.0f, -1.0f);

		md.normales += // upper
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
			Point3d(1.0f, 0.0f, 1.0f),
			Point3d(1.0f, 0.0f, 1.0f),
			Point3d(1.0f, 0.0f, 1.0f),
			Point3d(1.0f, 0.0f, 1.0f);

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
WagonDrawableObject::Render(ModelData & md)
{
	// lower
	md.points.push_back(Point3d(-0.7f, -0.7f, 0.0f));
	md.points.push_back(Point3d(0.7f, -0.7f, 0.0f));
	md.points.push_back(Point3d(0.7f, 0.7f, 0.0f));
	md.points.push_back(Point3d(-0.7f, 0.7f, 0.0f));

	// upper
	md.points.push_back(Point3d(-0.7f, -0.7f, 0.7f));
	md.points.push_back(Point3d(0.7f, -0.7f, 0.7f));
	md.points.push_back(Point3d(0.7f, 0.7f, 0.7f));
	md.points.push_back(Point3d(-0.7f, 0.7f, 0.7f));

#ifdef DRAWING_MODE_FULL
	RenderTriangles(md);

	md.type = ModelData::Mode::Triangle;
#else
	RenderLines(md);

	md.type = ModelData::Mode::Line;
#endif // DRAWING_MODE_FULL
	
	std::transform(md.points.begin(), md.points.end(), md.points.begin(), 
		[](decltype(*md.points.cbegin()) & p)
	{
		return p + Point3d(0.7f, 0.0f, 0.0f);
	});
	
	CalculateLength(md);
}
