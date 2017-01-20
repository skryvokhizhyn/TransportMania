
#pragma warning(disable: 4512 4100 6011 4503)

#include <boost/geometry/strategies/strategies.hpp>

#include "PolygonVisibilityChecker.h"

#include "Matrix.h"
#include "Point2d.h"
#include "Point3d.h"
#include "Point4d.h"
#include "MatrixUtils.h"

#include <boost/range/algorithm/transform.hpp>

#include <boost/geometry/io/wkt/read.hpp>

#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include <boost/geometry/algorithms/make.hpp>
#include <boost/geometry/algorithms/relation.hpp>

using namespace trm;

namespace
{
	namespace bg = boost::geometry;

	typedef bg::model::d2::point_xy<double> BGPoint2d;
	typedef bg::model::linestring<BGPoint2d> LineString2d;
	//typedef bg::model::box<BGPoint2d> Box2d;
	typedef bg::model::ring<BGPoint2d> Ring2d;

	/*Box2d GetEthalonVisibleScreen()
	{
		Box2d ethalon;
		bg::read_wkt("box(-1.0 -1.0, 1.0 1.0)", ethalon);

		return ethalon;
	}*/

	Ring2d GetEthalonVisibleScreenAsRing()
	{
		Ring2d ethalon;
		bg::read_wkt("polygon((-1.0 -1.0, -1.0 1.0, 1.0 1.0, 1.0 -1.0, -1.0 -1.0))", ethalon);

		return ethalon;
	}

	//const Box2d ETHALON_VISIBLE_SCREEN = GetEthalonVisibleScreen();
	const Ring2d ETHALON_VISIBLE_SCREEN_RING = GetEthalonVisibleScreenAsRing();

	Polygon3d GetConvertedPolygon(const trm::Matrix & pv, const trm::Polygon3d & points)
	{
		Polygon3d converted(points.size());

		boost::transform(points, converted.begin(),
			[&](const Point3d & p)
		{
			return pv * p;
		});

		return converted;
	}

	using CoordGetterFunc = const Point3d::Type (Point3d::*)() const;

	/*LineString2d GetProjection(const Polygon3d & p, CoordGetterFunc v1, CoordGetterFunc v2)
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
	}*/

	Ring2d GetProjectionAsRing(const Polygon3d & p, CoordGetterFunc v1, CoordGetterFunc v2)
	{
		Ring2d projection;
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

// it translates 3d point from world coordinates to pseudo 3d point
// where x and y are in [-1, 1] but z is close to 1 (depends on near frustrum value)
// so z coordinate doesn't take part in visibility check
bool
trm::CheckPolygonIsVisible(const trm::Matrix & pv, const trm::Polygon3d & points)
{
	const Polygon3d converted = GetConvertedPolygon(pv, points);
	
	const Ring2d xyRingProjection = GetProjectionAsRing(converted, &Point3d::x, &Point3d::y);

	const auto relation = bg::relation(ETHALON_VISIBLE_SCREEN_RING, xyRingProjection);

	//const LineString2d xyProjection = GetProjection(converted, &Point3d::x, &Point3d::y);
	//return bg::intersects(ETHALON_VISIBLE_SCREEN, xyProjection);;

	return relation[0 /*https://en.wikipedia.org/wiki/DE-9IM II section*/] == '2';
}
