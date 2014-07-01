#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "TerrainSceneOBject.h"
#include "DrawContext.h"
#include "WorldProjection.h"
#include "Terrain.h"
#include "StaticSceneObject.h"
#include "DynamicSceneObject.h"
#include "RoadNetwork.h"
#include "RailRoad.h"
#include "Train.h"
#include "TransportManager.h"
#include <functional>

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
		void RenderScene();

		bool IsStopped() const;
		void Stop();

		void ShiftScene(const float x, const float y);
		void ZoomScene(const float z);
		void RotateScene(const float angle);
		void BendScene(const float dtheta, const float dpsi);

		void Upper(const AxisType x, const AxisType y, const AxisType radii);

		void PutRailRoadLine();
		void PutRailRoadLine(const Point3d & from, const Point3d & to);
		void PutRailRoadArc();
		void PutRailRoadArc(const Point3d & from, const Point2d & c, const Angle a, const Direction d);

		//void EmulateDynamicScene1();
		void EmulateDynamicScene2();

	private:
		typedef std::vector<StaticSceneObjectPtr> StaticSceneObjects;
		typedef std::vector<DynamicSceneObjectPtr> DynamicSceneObjects;
		typedef std::vector<RoadRoutePtr> RoadRoutePtrs;
		//typedef std::vector<TrainPtr> TrainPark;
		//typedef std::function<bool ()> ManipulatorType;
		//typedef std::vector<ManipulatorType> Manipulators;
		typedef std::unique_ptr<TransportManager> TransportManagerUPtr;
		typedef std::vector<TransportManagerUPtr> TransportManagers;

	private:
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
		DynamicSceneObjects dynamicSceneObjects_;
		RoadNetwork roadNetwork_;
		RoadRoutePtrs roadRoutePtrs_;
		//TrainPark activeTrains_;
		//Manipulators manipulators_;
		TransportManagers managers_;
	};

} // namespace trm

#endif // _APPLICATION_H_
