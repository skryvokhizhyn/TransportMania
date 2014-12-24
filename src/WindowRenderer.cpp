#include "WindowRenderer.h"
#include "ModelData.h"

// assignment operator could not be generated
#pragma warning(disable: 4512)

#include <boost/assign/std/vector.hpp>

using namespace trm;

ModelData
WindowRenderer::RenderSquareWindow(const float size, const TextureId id)
{
	using namespace boost::assign;
	
	ModelData md;
	md.textureId = id;
	md.type = ModelData::Mode::Triangle;

	md.points.reserve(4);
	md.indexes.reserve(6);
	md.textures.reserve(4);

	md.points +=
		Point3d(0, 0, 0),
		Point3d(0, size, 0),
		Point3d(size, size, 0),
		Point3d(size, 0, 0);

	md.indexes += 0, 2, 1, 0, 3, 2;

	md.textures +=
		Point2d(0, 1),
		Point2d(0, 0),
		Point2d(1, 0),
		Point2d(1, 1);

	md.normales.resize(4, Point3d(0, 0, 1));

	return md;
}