#include "LocomotiveDrawableObject.h"
#include "ModelData.h"
#include "GlobalDefines.h"
#include "ModelManagerProxy.h"

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
}

void
LocomotiveDrawableObject::Render(ModelData & md)
{
#ifdef DRAWING_MODE_FULL
	md = ModelManagerProxy()->Get(TextureId::Wagon);

	md.type = ModelData::Mode::Triangle;
#else
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

	RenderLines(md);

	md.type = ModelData::Mode::Line;

	std::transform(md.points.cbegin(), md.points.cend(), md.points.begin(),
		[](decltype(*md.points.cbegin()) & p)
	{
		return p + Point3d(0.7f, 0.0f, 0.0f);
	});
#endif // DRAWING_MODE_FULL

	UpdateData(md);
	CalculateLength(md);
}
