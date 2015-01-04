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
#include "TextManager.h"
#include "TextureManager.h"
#include "WindowManager.h"
#include "EventHandlerLocator.h"
#include "SceneEventHandler.h"

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
		void Commit();
		
		// invalidates terrain so it starts tasselation
		void UpdateTerrain();
		// pauses updates of terrain
		void StopTerrainUpdate();
		// resumes updates of terrain
		void ResumeTerrainUpdate();

		void Upper(const AxisType x, const AxisType y, const AxisType radii);

		void PutRailRoadLine(const Point3d & from, const Point3d & to);
		void PutRailRoadArc(const Point3d & from, const Point2d & c, const Angle a, const Rotation r);

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
		bool stopped_ = false;
		DrawContext context_;
		WorldProjection worldProjection_;
		TerrainPtr terrainPtr_;
		TerrainSceneObjectPtr terrainScenePtr_;
		StaticSceneObjects staticSceneObjects_;
		RoadNetwork roadNetwork_;
		RoadRoutePtrs roadRoutePtrs_;
		TransportManagers managers_;
		TextManager textManager_;
		ComponentHolder componentHolder_;
		TextureManager textureManager_;
		WindowManager windowManager_;
		EventHandlerLocator eventHandlerLocator_;
		SceneEventHandlerPtr sceneHandlerPtr_;
		bool processTerrainUpdate_ = true;
	};

} // namespace trm

#endif // _APPLICATION_H_
