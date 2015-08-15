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

	sceneHandlerPtr_ = std::make_shared<MoveSceneEventHandler>();
	sceneHandlerPtr_->SetMoveHandler(*this);
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

	sceneContent_.Init(SceneContent::Type::Init);

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
	if (processTerrainUpdate_)
		terrainScenePtr_->Update(worldProjection_);

	if (!paused_)
	{
		boost::for_each(managers_, std::bind(&TransportManager::Update, std::placeholders::_1));
	}

	componentHolder_.Update(worldProjection_);
}

void 
Application::Render()
{
	if (processTerrainUpdate_)
		terrainScenePtr_->Render(worldProjection_);
}

void 
Application::Draw()
{
	context_.Clear();

	context_.Activate(ShaderProgramType::Terrain);

	const Matrix & pvm = worldProjection_.GetProjectionViewMatrix();

	context_.Transform(pvm, terrainScenePtr_->GetModelMatrix());
	terrainScenePtr_->Draw();

	// uses context transformation of terrain so no Transform method call
	std::for_each(staticSceneObjects_.cbegin(), staticSceneObjects_.cend(), 
		[](const StaticSceneObjectPtr & ssoPtr){ssoPtr->Draw();});

	// uses context transformation of terrain so no Transform method call
	std::for_each(tempRoadObjects_.cbegin(), tempRoadObjects_.cend(), 
		[](const StaticSceneObjectPtr & ssoPtr){ssoPtr->Draw();});

	componentHolder_.Draw(context_, pvm);

	context_.Activate(ShaderProgramType::Window);

	const Matrix & ovm = worldProjection_.GetOrthoViewMatrix();
	textManager_.Draw(context_, ovm);
	windowManager_.Draw(context_, ovm);
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
		Terraformer te(range, *tf.get());
		terrainPtr_->Apply(te);

		terrainScenePtr_->UpdateRequired();
	}
}

void
Application::Commit()
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

void 
Application::Upper(const AxisType /*x*/, const AxisType /*y*/, const AxisType /*radii*/)
{
	//TerrainRangeLine range(trm::Point2d(x, y), trm::Point2d(x + 10.0f, y), radii);
	//TerrainRangeArc range(TerrainRangeArc::Data(Point2d(5, 15), Degrees(150), Point2d(15, 15), Rotation::AntiClockwise), 1);
	//TerrainRangeCircle range(trm::Point2d(x, y), radii);

	//TerrainRangeLine range(trm::Point2d(10, 10), trm::Point2d(50, 10), 10);

	/*auto tf = TerraformFunctionFactory::GetConstant(0.0f);
	Terraformer t(range, *tf.get());
	terrainPtr_->Apply(t);

	terrainScenePtr_->UpdateRequired();*/
}

void 
Application::PutRailRoad(const RailRoadPtr & rrp)
{
	if (!roadNetworkManager_.InsertPermanentRoad(rrp))
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

	staticSceneObjects_.push_back(StaticSceneObjectFactory::ForRailRoad(rrp));

	terrainScenePtr_->UpdateRequired();
}

void 
Application::PutRailRoadLine(const Point3d & from, const Point3d & to)
{
	const RailRoadPtr rrp = RailRoadFactory::Line(from, to);
	PutRailRoad(rrp);
}

void 
Application::PutRailRoadArc(const Point3d & from, const Point2d & c, const Angle a, const Rotation r)
{
	const RailRoadPtr rrp = RailRoadFactory::Arc(from, a, c, r);
	PutRailRoad(rrp);
}

void 
Application::EmulateDynamicScene1()
{
	const Point3d p1(30, 10, 6);
	const Point3d p2(50, 10, 13);
	const Point3d p3(10, 30, 6);
	
	//const RailRoadPtr rrp = RailRoadFactory::Line(p1, p2);
	
	//roadNetwork_.Insert(rrp);

	PutRailRoadLine(p1, p2);
	PutRailRoadArc(p3, Point2d(30, 30), Degrees(90), Rotation::AntiClockwise);

	const RoadRoutePtr rrPtr = roadNetworkManager_.GetRoute(p3, p2);

	managers_.emplace_back(RoadRouteHolder(rrPtr, Heading::Forward));
}

void 
Application::EmulateDynamicScene2()
{
	const Point3d p1(10, 10, 3);
	const Point3d p2(30, 10, 3);
	const Point2d c1(30, 20);
	const Angle a = Degrees(180);
	const Point3d p3(30, 30, 3);
	const Point3d p4(20, 30, 3);
	const Point2d c2(20, 40);
	const Point3d p5(20, 50, 3);
	const Point3d p6(40, 50, 3);

	PutRailRoadLine(p1, p2);
	PutRailRoadArc(p2, c1, a, Rotation::AntiClockwise);
	PutRailRoadLine(p3, p4);
	PutRailRoadArc(p4, c2, a, Rotation::Clockwise);
	PutRailRoadLine(p5, p6);
	 
	const RoadRoutePtr rrPtr = roadNetworkManager_.GetRoute(p1, p6);

	roadRoutePtrs_.push_back(rrPtr);
}

void
Application::CloseWindow(UniqueId id)
{
	windowManager_.CloseWindow(id);
}

void
Application::Pause()
{
	paused_ = !paused_;
}

void
Application::ChangeMouseMode()
{
	sceneHandlerPtr_->ChangeHandler(*this);
}

void 
Application::SubmitDraftRoads(bool yesNo)
{
	RailRoadPtrs tempRoads = roadNetworkManager_.GetTemporaryRoads();
	tempRoadObjects_.clear();

	if (yesNo)
	{
		roadNetworkManager_.CommitIntersections();
		boost::for_each(tempRoads, boost::bind(&Application::PutRailRoad, this, _1));
	}

	roadNetworkManager_.ClearTemporaryData();
	sceneContent_.Close(SceneContent::Type::Draw);
}

void 
Application::PutRoad(const Point2d & from, const Point2d & to, bool commit)
{
	if (commit)
	{
		const auto foundFrom = GetTerrainPosition(from, worldProjection_, screenConverter_, terrainPtr_);
		const auto foundTo = GetTerrainPosition(to, worldProjection_, screenConverter_, terrainPtr_);

		if (foundFrom && foundTo && foundFrom != foundTo)
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
	if (!roadNetworkManager_.InsertTemporaryRoad(rrp))
	{
		return;
	}

	if (tempRoadObjects_.empty())
	{
		sceneContent_.Init(SceneContent::Type::Draw);
	}

	RailRoadRangeGenerator rrrg(true);
	rrp->Accept(rrrg);

	TerrainPointCollector tpc;
	Terraformer t(rrrg.GetRange(), tpc);
	terrainPtr_->Apply(t);

	tempRoadObjects_.push_back(StaticSceneObjectFactory::ForTerrainCover(tpc.GetPoints()));
}
//
//void 
//Application::PutLineDraft(const Point3d & from, const Point3d & to)
//{
//	const RailRoadPtr rrp = RailRoadFactory::Line(from, to);
//	PutRoadDraft(rrp);
//}
