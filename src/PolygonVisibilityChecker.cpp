#pragma warning(push)
#pragma warning(disable: 4512 4100)

#include <boost/geometry/strategies/strategies.hpp>

#include "PolygonVisibilityChecker.h"

#include "Matrix.h"
#include "Point2d.h"
#include "Point3d.h"
#include "Point4d.h"
#include "MatrixUtils.h"

#include <boost/range/algorithm/transform.hpp>
#include <boost/geometry/io/wkt/read.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/algorithms/make.hpp>
#include <boost/geometry/algorithms/intersects.hpp>
#include <boost/geometry/algorithms/correct.hpp>

using namespace trm;

using Polygon2d = std::vector<Point2d>;

namespace
{
	namespace bg = boost::geometry;

	typedef bg::model::d2::point_xy<double> BGPoint2d;
	typedef bg::model::linestring<BGPoint2d> LineString2d;

	LineString2d GetEthalonVisibleScreen()
	{
		LineString2d ethalon;
		bg::read_wkt("linestring(-1.0 -1.0, 1.0 -1.0, 1.0 1.0, -1.0 1.0, -1.0 -1.0)", ethalon);

		return ethalon;
	}

	const LineString2d ETHALON_VISIBLE_SCREEN = GetEthalonVisibleScreen();

	Polygon3d GetConvertedPolygon(const trm::Matrix & pv, const trm::Polygon3d & points)
	{
		Polygon3d converted;
		converted.reserve(points.size());

		boost::transform(points, std::back_inserter(converted),
			[&](const Point3d & p)
		{
			Point4d p4d = Point4d::Cast(p);
			p4d.w() = 1.0f;

			p4d = pv * p4d;
			p4d /= p4d.w();

			return Point3d::Cast(p4d);
		});

		return converted;
	}

	using CoordGetterFunc = const Point3d::Type (Point3d::*)() const;

	LineString2d GetProjection(const Polygon3d & p, CoordGetterFunc v1, CoordGetterFunc v2)
	{
		LineString2d projection;
		projection.reserve(p.size() + 1);
		boost::transform(p, std::back_inserter(projection),
			[=](const Point3d & p3d)
		{
			return bg::make<BGPoint2d>((p3d.*v1)(), (p3d.*v2)());
		});
		projection.push_back(projection[0]);

		return projection;
	}
}

bool
trm::CheckPolygonIsVisible(const trm::Matrix & pv, const trm::Polygon3d & points)
{
	const Polygon3d converted = GetConvertedPolygon(pv, points);

	const LineString2d xzProjection = GetProjection(converted, &Point3d::x, &Point3d::z);

	const bool xzIntersects = bg::intersects(ETHALON_VISIBLE_SCREEN, xzProjection);

	if (!xzIntersects)
		return false;

	const LineString2d xyProjection = GetProjection(converted, &Point3d::x, &Point3d::y);

	return bg::intersects(ETHALON_VISIBLE_SCREEN, xyProjection);
}

#pragma warning(pop)
