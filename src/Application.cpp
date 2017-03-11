#include "Application.h"
#include "Terraformer.h"
#include "Point2d.h"
#include "TerrainRangeCircle.h"
#include "TerraformFunction.h"
#include "HeightMapLoaderFactory.h"
#include "RailRoadFactory.h"
#include "StaticSceneObjectFactory.h"
#include "Settings.h"
#include "Angle.h"
#include "Logger.h"
#include "TerrainPositionLocator.h"
#include "TerrainPointCollector.h"
#include "SceneContent.h"

#include "RailRoadRangeGenerator.h"
#include "RailRoadTerraformer.h"
#include "RailRoadConnectionResult.h"
#include "RailRoadUpdateEvent.h"

#include "TextManagerProxy.h"
#include "ComponentHolderProxy.h"
#include "TextureManagerProxy.h"
#include "WindowManagerProxy.h"
#include "CachedHandlerLocatorProxy.h"
#include "TextRendererProxy.h"
#include "ModelManagerProxy.h"

#include <boost/range/algorithm.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/lambda/bind.hpp>

using namespace trm;
using namespace trm::terrain;

Application::Application()
{
}

bool 
Application::InitApplication(const size_t width, const size_t height)
{
	stopped_ = false;

	worldProjection_.SetRatio(width, height);
	worldProjection_.SetAngles(Degrees(0), Degrees(0), Degrees(0));
	//worldProjection_.SetAngles(Degrees(55), Degrees(0), Degrees(10));
	worldProjection_.SetShift(Point3d(30, 30, 100));
	
	textManager_.Init(Size2d(width, height));
	textRenderer_.Init();

	sceneHandlerPtr_ = std::make_shared<MoveSceneEventHandler>(*this);

	screenConverter_.SetScreenSize(Size2d(width, height));

	cachedHandlerLocator_.Put(UniqueId::Generate(), screenConverter_.GetScreenPolygon(), 
		HandlerLevel::Terrain, sceneHandlerPtr_);
	
	windowManager_.Init(Size2d(width, height));
	sceneContent_.SetWindowManager(windowManager_);

	TextManagerProxy::Init(textManager_);
	ComponentHolderProxy::Init(componentHolder_);
	TextureManagerProxy::Init(textureManager_);
	CachedHandlerLocatorProxy::Init(cachedHandlerLocator_);
	TextRendererProxy::Init(textRenderer_);
	ModelManagerProxy::Init(modelManager_);

	sceneContent_.PutPauseGoButton(paused_);
	ChangeMouseMode(MoveSceneEventHandlerType::Move);

	return true;
}

bool 
Application::InitView()
{
	context_.Init();

	HeightMapLoaderPtr hmlPtr = 
		terrain::HeightMapLoaderFactory::GetFileLoader(trm::GetHeightMapPath("hf_513.bmp"));
	HeightMapLoaderPtr hmlCachedPtr =
		terrain::HeightMapLoaderFactory::GetCachedLoader(Terrain::PATCH_SIZE, hmlPtr);
	
	terrainPtr_.reset(new Terrain(hmlCachedPtr));

	terrainScenePtr_ = std::make_shared<TerrainSceneObject>(terrainPtr_);
	
	return true;
}

bool 
Application::ReleaseView()
{
	// all rendering objects have to be cleared here
	tempRoadObjects_.clear();
	staticSceneObjects_.clear();
	terrainScenePtr_.reset();
	context_.Term();

	return true;
}

bool 
Application::QuitApplication()
{
	ModelManagerProxy::Term();
	TextRendererProxy::Term();
	CachedHandlerLocatorProxy::Term();
	TextureManagerProxy::Term();
	ComponentHolderProxy::Term();
	TextManagerProxy::Term();
	
	return true;
}

bool
Application::IsStopped() const
{
	return stopped_;
}

void
Application::Stop()
{
	stopped_ = true;
}

void 
Application::Update()
{
	if (!paused_)
	{
		boost::for_each(managers_, std::bind(&TransportManager::Update, std::placeholders::_1));
	}

	componentHolder_.Update(worldProjection_);
}

void 
Application::Draw()
{
	context_.Clear();

	context_.Activate(ShaderProgramType::Terrain);

	const Matrix & pvm = worldProjection_.GetProjectionViewMatrix();

	context_.SetLightPosition(worldProjection_.GetCameraPosition());

	context_.Transform(pvm, terrainScenePtr_->GetModelMatrix());
	terrainScenePtr_->Draw();

	// uses context transformation of terrain so no Transform method call
	std::for_each(staticSceneObjects_.cbegin(), staticSceneObjects_.cend(), 
		[](const StaticSceneObjects::value_type & sso){sso.second->Draw();});

	// uses context transformation of terrain so no Transform method call
	std::for_each(tempRoadObjects_.cbegin(), tempRoadObjects_.cend(), 
		[](const StaticSceneObjects::value_type & sso){sso.second->Draw();});

	componentHolder_.Draw(context_, pvm);

	context_.Activate(ShaderProgramType::Window);

	const Matrix & ovm = worldProjection_.GetOrthoViewMatrix();
	textManager_.Draw(context_, ovm);
	windowManager_.Draw(context_, ovm);
}

void trm::Application::Render()
{
	if (processTerrainUpdate_)
	{
		terrainScenePtr_->Render(worldProjection_);
	}
}

void 
Application::ShiftScene(const float x, const float y)
{
	worldProjection_.Shift(-x, -y);
}

void 
Application::ZoomScene(const float z)
{
	worldProjection_.Zoom(-z);
}

void 
Application::RotateScene(const Angle angle)
{
	worldProjection_.Rotate(-angle);
}

void 
Application::BendScene(const Angle dtheta, const Angle dpsi)
{
	worldProjection_.Bend(dtheta, dpsi);
}

void 
Application::PressScene(const float x, const float y)
{
	auto found = GetTerrainPosition(Point2d(x, y),
		worldProjection_, screenConverter_, terrainPtr_);

	if (found)
	{
		TerrainRangeCircle range(Point2d::Cast(found.get()), 1);
		
		auto tf = TerraformFunctionFactory::GetIncrease(1.0f);
		Terraformer te(range, *tf);
		terrainPtr_->Apply(te);

		terrainScenePtr_->UpdateRequired();
	}
}

void
Application::CommitSceneActions()
{
	sceneHandlerPtr_->Commit();
}

void 
Application::UpdateTerrain()
{
	terrainScenePtr_->UpdateRequired();
}

void 
Application::StopTerrainUpdate()
{
	processTerrainUpdate_ = false;
}
		
void 
Application::ResumeTerrainUpdate()
{
	processTerrainUpdate_ = true;
}

//void 
//Application::Upper(const AxisType /*x*/, const AxisType /*y*/, const AxisType /*radii*/)
//{
//	TerrainRangeLine range(trm::Point2d(x, y), trm::Point2d(x + 10.0f, y), radii);
//	TerrainRangeArc range(TerrainRangeArc::Data(Point2d(5, 15), Degrees(150), Point2d(15, 15), Rotation::AntiClockwise), 1);
//	TerrainRangeCircle range(trm::Point2d(x, y), radii);
//
//	TerrainRangeLine range(trm::Point2d(10, 10), trm::Point2d(50, 10), 10);
//
//	/*auto tf = TerraformFunctionFactory::GetConstant(0.0f);
//	Terraformer t(range, *tf.get());
//	terrainPtr_->Apply(t);
//
//	terrainScenePtr_->UpdateRequired();*/
//}

//void 
//Application::PutRailRoad(const RailRoadPtr & rrp)
//{
//	OptionalUniqueId roadId = roadNetworkManager_.InsertPermanentRoad(rrp);
//	
//	if (!roadId)
//	{
//		return;
//	}
//
//	DrawPermanentRailRoad(roadId.get());
//}

void 
Application::DrawPermanentRailRoad(UniqueId id)
{
	const RailRoadPtr & rrp = roadNetworkManager_.GetRoadById(id);

	if (!rrp)
	{
		return;
	}

	RailRoadRangeGenerator rrrg(true);
	rrp->Accept(rrrg);

	RailRoadTerraformer rrtf;
	rrp->Accept(rrtf);

	TerraformFunctionPtr tFuncPtr = rrtf.GetTerraformer();

	Terraformer t(rrrg.GetRange(), *tFuncPtr);
	terrainPtr_->Apply(t);

	staticSceneObjects_.emplace(id, StaticSceneObjectFactory::ForRailRoad(rrp));

	terrainScenePtr_->UpdateRequired();
}

//void 
//Application::PutRailRoadLine(const Point3d & from, const Point3d & to)
//{
//	const RailRoadPtr rrp = RailRoadFactory::Line(from, to);
//	PutRailRoad(rrp);
//}
//
//void 
//Application::PutRailRoadArc(const Point3d & from, const Point2d & c, const Angle a)
//{
//	const RailRoadPtr rrp = RailRoadFactory::Arc(from, a, c);
//	PutRailRoad(rrp);
//}

//void 
//Application::EmulateDynamicScene1()
//{
//	const Point3d p1(30, 10, 6);
//	const Point3d p2(50, 10, 13);
//	const Point3d p3(10, 30, 6);
//	
//	//const RailRoadPtr rrp = RailRoadFactory::Line(p1, p2);
//	
//	//roadNetwork_.Insert(rrp);
//
//	PutRailRoadLine(p1, p2);
//	PutRailRoadArc(p3, Point2d(30, 30), Degrees(90));
//
//	const RoadRoutePtr rrPtr = roadNetworkManager_.GetRoute(p3, p2);
//
//	managers_.emplace_back(RoadRouteHolder(rrPtr, Heading::Forward));
//}
//
//void 
//Application::EmulateDynamicScene2()
//{
//	const Point3d p1(10, 10, 3);
//	const Point3d p2(30, 10, 3);
//	const Point2d c1(30, 20);
//	const Angle a = Degrees(180);
//	const Point3d p3(30, 30, 3);
//	const Point3d p4(20, 30, 3);
//	const Point2d c2(20, 40);
//	const Point3d p5(20, 50, 3);
//	const Point3d p6(40, 50, 3);
//
//	PutRailRoadLine(p1, p2);
//	PutRailRoadArc(p2, c1, a);
//	PutRailRoadLine(p3, p4);
//	PutRailRoadArc(p4, c2, a * -1.0f);
//	PutRailRoadLine(p5, p6);
//	 
//	const RoadRoutePtr rrPtr = roadNetworkManager_.GetRoute(p1, p6);
//
//	roadRoutePtrs_.push_back(rrPtr);
//}

void
Application::CloseWindow(UniqueId id)
{
	windowManager_.CloseWindow(id);
}

void
Application::Pause()
{
	paused_ = !paused_;

	sceneContent_.PutPauseGoButton(paused_);
}

void
Application::ChangeMouseMode(MoveSceneEventHandlerType type)
{
	sceneHandlerPtr_->ChangeHandler(type);
	
	sceneContent_.PutMouseModeButton(type);
}

void 
Application::SubmitDraftRoads(bool yesNo)
{
	tempRoadObjects_.clear();

	if (yesNo)
	{
		RailRoadUpdateEvent(roadNetworkManager_.CommitTemporaryRoads());
	}
	else
	{
		roadNetworkManager_.ClearTemporaryData();
	}
}

void 
Application::PutRoad
//(const Point2d & /*from*/, const Point2d & /*to*/, bool commit)
(const Point2d & from1, const Point2d & to1, bool commit)
{
	//Point2d from(212, 519);
	//Point2d to(524, 292);
	//198, 546; 533, 526
	
	Point2d from = from1;
	Point2d to = to1;

	/*Point2d from;
	Point2d to;*/

	if (commit)
	{
		utils::Logger().Trace() << from1 << to1;

		/*static int i = 0;

		if (i == 0)
		{
			from = Point2d(208, 537);
			to = Point2d(683, 512);
		}
		else
		{
			from = Point2d(482, 524);
			to = Point2d(803, 212);
		}

		++i;*/

		const auto foundFrom = GetTerrainPosition(from, worldProjection_, screenConverter_, terrainPtr_);
		const auto foundTo = GetTerrainPosition(to, worldProjection_, screenConverter_, terrainPtr_);

		if (foundFrom && foundTo)
		{
			const RailRoadConnectionResult connectionResult = roadNetworkManager_.CreateRoad(*foundFrom, *foundTo);

			boost::for_each(connectionResult.roadPtrs, boost::bind(&Application::PutRoadDraft, this, _1));
			roadNetworkManager_.InsertTemporaryIntersections(connectionResult.intersestions);
		}
	}
}

void 
Application::PutRoadDraft(const RailRoadPtr & rrp)
{
	OptionalUniqueId roadId = roadNetworkManager_.InsertTemporaryRoad(rrp);

	if (!roadId)
	{
		return;
	}

	RailRoadAffectedIds ids(RailRoadAffectedIds::Temporary);
	ids.addedIds.push_back(roadId.get());

	RailRoadUpdateEvent(ids);
}

void 
Application::DrawTemporaryRailRoad(UniqueId id)
{
	const RailRoadPtr & rrp = roadNetworkManager_.GetRoadById(id);
	
	if (!rrp)
	{
		return;
	}

	if (tempRoadObjects_.empty())
	{
		sceneContent_.PutSubmitDraftRoadButtons();
	}

	RailRoadRangeGenerator rrrg(true);
	rrp->Accept(rrrg);

	TerrainPointCollector tpc;
	Terraformer t(rrrg.GetRange(), tpc);
	terrainPtr_->Apply(t);

	tempRoadObjects_.emplace(id, StaticSceneObjectFactory::ForTerrainCover(tpc.GetPoints()));
}

void 
Application::RemoveDrawnRailRoad(UniqueId id)
{
	staticSceneObjects_.erase(id);
}

void 
Application::ActualizeRenderedData()
{
	terrainScenePtr_->Actualize();
}
