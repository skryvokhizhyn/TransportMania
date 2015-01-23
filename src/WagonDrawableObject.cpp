#include "WagonDrawableObject.h"
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
		md.indexes += 0, 1, 1, 2, 2, 3, 3, 0;
		md.indexes += 4, 5, 5, 6, 6, 7, 7, 0;
		md.indexes += 0, 4, 1, 5, 2, 6, 3, 7;
	}
}

void
WagonDrawableObject::Render(ModelData & md)
{
#ifdef DRAWING_MODE_FULL
	md = ModelManagerProxy()->Get(TextureId::Wagon);
#else
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

	RenderLines(md);

	md.type = ModelData::Mode::Line;

	std::transform(md.points.begin(), md.points.end(), md.points.begin(), 
		[](decltype(*md.points.cbegin()) & p)
	{
		return p + Point3d(0.7f, 0.0f, 0.0f);
	});
#endif // DRAWING_MODE_FULL
	
	UpdateData(md);
	CalculateLength(md);
}
