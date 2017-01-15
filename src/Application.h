#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "TerrainSceneObject.h"
#include "DrawContext.h"
#include "WorldProjection.h"
#include "Terrain.h"
#include "StaticSceneObject.h"
#include "RoadNetworkManager.h"
#include "RailRoad.h"
#include "Train.h"
#include "TransportManager.h"
#include "ComponentHolder.h"
#include "TextManager.h"
#include "TextRenderer.h"
#include "TextureManager.h"
#include "WindowManager.h"
#include "CachedHandlerLocator.h"
#include "MoveSceneEventHandler.h"
#include "ModelManager.h"
#include "ScreenSizeManipulator.h"
#include "SceneContent.h"

namespace trm
{
	class StaticSceneObject;
	class TerrainRange;
	struct Point3d;

	class Application
	{
	public:
		Application();

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
		void PressScene(const float x, const float y);
		void CommitSceneActions();
		
		// invalidates terrain so it starts tasselation
		void UpdateTerrain();
		// pauses updates of terrain
		void StopTerrainUpdate();
		// resumes updates of terrain
		void ResumeTerrainUpdate();

		void Upper(const AxisType x, const AxisType y, const AxisType radii);

		void PutRoad(const Point2d & from, const Point2d & to, bool commit);
		void PutRailRoadLine(const Point3d & from, const Point3d & to);
		void PutRailRoadArc(const Point3d & from, const Point2d & c, const Angle a);
		void PutRailRoad(const RailRoadPtr & rrp);
		void PutRoadDraft(const RailRoadPtr & rrp);
		//void PutLineDraft(const Point3d & from, const Point3d & to);

		void EmulateDynamicScene1();
		void EmulateDynamicScene2();

		void CloseWindow(UniqueId id);
		void Pause();
		void ChangeMouseMode(MoveSceneEventHandlerType type);
		void SubmitDraftRoads(bool yesNo);
		void DrawPermanentRailRoad(UniqueId id);
		void DrawTemporaryRailRoad(UniqueId id);
		void ActualizeRenderedData();

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
		StaticSceneObjects tempRoadObjects_;
		RoadNetworkManager roadNetworkManager_;
		RoadRoutePtrs roadRoutePtrs_;
		TransportManagers managers_;
		TextManager textManager_;
		TextRenderer textRenderer_;
		ComponentHolder componentHolder_;
		TextureManager textureManager_;
		WindowManager windowManager_;
		CachedHandlerLocator cachedHandlerLocator_;
		MoveSceneEventHandlerPtr sceneHandlerPtr_;
		ModelManager modelManager_;
		ScreenSizeManipulator screenConverter_;
		SceneContent sceneContent_;
		bool processTerrainUpdate_ = true;
		bool paused_ = false;
	};

} // namespace trm

#endif // _APPLICATION_H_
