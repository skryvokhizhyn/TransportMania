#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "TerrainSceneObject.h"
#include "DrawContext.h"
#include "WorldProjection.h"
#include "Terrain.h"
#include "StaticSceneObject.h"
#include "RoadNetwork.h"
#include "RailRoad.h"
#include "Train.h"
#include "TransportManager.h"
#include "ComponentHolder.h"
#include "TextManagerHub.h"

namespace trm
{
	class StaticSceneObject;
	struct Point3d;

	class Application
	{
	public:
		bool InitApplication(const size_t width, const size_t height);
		bool InitView();
		bool ReleaseView();
		bool QuitApplication();

		bool IsStopped() const;
		void Stop();

		void ShiftScene(const float x, const float y);
		void ZoomScene(const float z);
		void RotateScene(const Angle angle);
		void BendScene(const Angle dtheta, const Angle dpsi);

		void Upper(const AxisType x, const AxisType y, const AxisType radii);

		void PutRailRoadLine();
		void PutRailRoadLine(const Point3d & from, const Point3d & to);
		void PutRailRoadArc();
		void PutRailRoadArc(const Point3d & from, const Point2d & c, const Angle a, const Direction d);

		void EmulateDynamicScene1();
		void EmulateDynamicScene2();

	private:
		typedef std::vector<StaticSceneObjectPtr> StaticSceneObjects;
		typedef std::vector<RoadRoutePtr> RoadRoutePtrs;
		typedef std::vector<TransportManager> TransportManagers;

	public:
		void Update();
		void Render();
		void Draw();

	private:
		bool stopped_;
		DrawContext context_;
		WorldProjection worldProjection_;
		TerrainPtr terrainPtr_;
		TerrainSceneObjectPtr terrainScenePtr_;
		StaticSceneObjects staticSceneObjects_;
		RoadNetwork roadNetwork_;
		RoadRoutePtrs roadRoutePtrs_;
		TransportManagers managers_;
		TextManagerHub textManagerHub_;
		ComponentHolder componentHolder_;
	};

} // namespace trm

#endif // _APPLICATION_H_
