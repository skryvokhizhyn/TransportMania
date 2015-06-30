#include "TerrainPositionLocator.h"
#include "Point2d.h"
#include "WorldProjection.h"
#include "ScreenSizeManipulator.h"
#include "GeometryUtils.h"
#include "TerrainRangeLine.h"
#include "TerrainHeightCheck.h"
#include "Terraformer.h"
#include "TerrainRangeCircle.h"

using namespace trm;

OptionalPoint3d 
trm::GetTerrainPosition(
	const Point2d & p,
	const WorldProjection & wp, 
	const ScreenSizeManipulator & sm,
	const TerrainPtr & tPtr)
{
	const Point3d & camera = wp.GetCameraPosition();
	const Point2d oglPoint = sm.ConvertToOgl(p);
	const Point3d pointIn3d = wp.ToWorldCoordinates(oglPoint);
	const Point3d vec = utils::Normalized(pointIn3d - camera) * 200.0f;

	TerrainRangeLine range(Point2d::Cast(pointIn3d), Point2d::Cast(camera + vec), 0.0f);
	TerrainHeightCheck thc(oglPoint, wp.GetProjectionViewMatrix());
	Terraformer t(range, thc);

	tPtr->Apply(t);

	return thc.Get();
}
