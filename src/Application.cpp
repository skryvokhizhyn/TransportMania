#include "Application.h"
#include "Terraformer.h"
#include "Point2d.h"
#include "TerrainRangeLine.h"
#include "TerrainRangeArc.h"
#include "TerrainRangeCircle.h"
#include "TerraformFunction.h"
#include "HeightMapLoaderFactory.h"
#include "RailRoadFactory.h"
#include "StaticSceneObjectFactory.h"
#include "Train.h"
#include "Settings.h"
#include "Angle.h"
#include "TrainVisibilityHandler.h"
#include <boost/range/algorithm.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/lambda/bind.hpp>

using namespace trm;
using namespace trm::terrain;

bool SCENE_UPDATED = false;

bool 
Application::InitApplication(const size_t width, const size_t height)
{
	stopped_ = false;

	if (height == 0)
		throw std::runtime_error("Zero screen height specified");

	worldProjection_.SetRatio(static_cast<float>(width) / height);
	//worldProjection_.SetAngles(Degrees(69), Degrees(0), Degrees(-32));
	//worldProjection_.SetAngles(Degrees(10), Degrees(0), Degrees(0));
	worldProjection_.SetAngles(Degrees(61), Degrees(0), Degrees(-6)); //9
	worldProjection_.SetShift(Point3d(30, 30, 100));
	
	return true;
}

bool 
Application::InitView()
{
	context_.Init();

	HeightMapLoaderPtr hmlPtr = terrain::HeightMapLoaderFactory::GetFileLoader(
		(boost::filesystem::path(trm::GetDataFolderPath()) / "hf_513.bmp").string());
	
	terrainPtr_.reset(new Terrain(hmlPtr));

	terrainScenePtr_ = std::make_shared<TerrainSceneObject>(terrainPtr_);

	return true;
}

bool 
Application::ReleaseView()
{
	// all rendering objects have to be cleared here
	staticSceneObjects_.clear();
	terrainScenePtr_.reset();
	context_.Term();

	return true;
}

bool 
Application::QuitApplication()
{
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
	if (!SCENE_UPDATED)
	{
		terrainScenePtr_->Update(worldProjection_);
	}

	boost::for_each(managers1_, std::bind(&TransportManager::Update, std::placeholders::_1));

	TrainVisibilityHandler visibilityHandler(worldProjection_, componentHolder_);
	boost::for_each(componentHolder_.movables | boost::adaptors::map_values,
		std::ref(visibilityHandler));
	
	auto it = boost::remove_if(componentHolder_.visibles,
		!boost::bind(&TrainVisibleObject::Update, boost::lambda::_1));

	componentHolder_.visibles.erase(it, componentHolder_.visibles.end());
}

void 
Application::Render()
{
	if (!SCENE_UPDATED)
	{	
		terrainScenePtr_->Render();
	}
}

void 
Application::Draw()
{
	const Matrix & pvm = worldProjection_.GetProjectionViewMatrix();

	context_.Transform(pvm, terrainScenePtr_->GetModelMatrix());
	terrainScenePtr_->Draw();

	// uses context transformation of terrain so no Transform method call
	std::for_each(staticSceneObjects_.cbegin(), staticSceneObjects_.cend(), 
		[](const StaticSceneObjectPtr & ssoPtr){ssoPtr->Draw();});

	boost::for_each(componentHolder_.visibles,
		[&](const TrainVisibleObject & tvo)
	{
		context_.Transform(pvm, tvo.GetMatrix());
		tvo.Draw();
	});
}

void 
Application::RenderScene()
{
	context_.Clear();

	Update();
	Render();
	Draw();	

	SCENE_UPDATED = true;
}

void 
Application::ShiftScene(const float x, const float y)
{
	worldProjection_.Shift(x, y);

	SCENE_UPDATED = false;
}

void 
Application::ZoomScene(const float z)
{
	worldProjection_.Zoom(z);

	SCENE_UPDATED = false;
}

void 
Application::RotateScene(const float angle)
{
	worldProjection_.Rotate(Degrees(angle));

	SCENE_UPDATED = false;
}

void 
Application::BendScene(const float dtheta, const float dpsi)
{
	worldProjection_.Bend(Degrees(dtheta), Degrees(dpsi));

	SCENE_UPDATED = false;
}

void 
Application::Upper(const AxisType /*x*/, const AxisType /*y*/, const AxisType /*radii*/)
{
	//TerrainRangeLine range(trm::Point2d(x, y), trm::Point2d(x + 10.0f, y), radii);
	TerrainRangeArc range(TerrainRangeArc::Data(Point2d(5, 15), Degrees(150), Point2d(15, 15), Direction::Right), 1);
	//TerrainRangeCircle range(trm::Point2d(x, y), radii);

	//TerrainRangeLine range(trm::Point2d(10, 10), trm::Point2d(50, 10), 10);

	//Terraformer t(range, TerraformFunctionFactory::GetAngleLinear(trm::Point2d(10, 10), trm::Point2d(30, 10), 1, 1.2f));
	Terraformer t(range, TerraformFunctionFactory::GetConstant(0.0f));
	terrainPtr_->Apply(t);

	SCENE_UPDATED = false;
}

void 
Application::PutRailRoadLine()
{
	const AxisType h = 3.0f;
	const Point3d from(10.5f, 10.5f, h);
	const Point3d to(2000, 30, h);
	
	PutRailRoadLine(from, to);
}

void 
Application::PutRailRoadLine(const Point3d & from, const Point3d & to)
{
	TerrainRangeLine range(Point2d::Cast(from), Point2d::Cast(to), RailRoad::GetTotalWidth());

	Terraformer t(range, TerraformFunctionFactory::GetConstant(from.z()));
	terrainPtr_->Apply(t);

	const RailRoadPtr rrp = RailRoadFactory::Line(from, to);

	if (!roadNetwork_.Insert(rrp))
	{
		return;
	}

	staticSceneObjects_.push_back(StaticSceneObjectFactory::ForRailRoad(rrp));
}

void 
Application::PutRailRoadArc()
{
	const AxisType h = 3.0f;
	const Point3d from(30, 10, h);
	const Point2d center(20, 100);
	const Angle a = Degrees(150);
	const Direction dir = Direction::Right;

	PutRailRoadArc(from, center, a, dir);
}

void 
Application::PutRailRoadArc(const Point3d & from, const Point2d & c, const Angle a, const Direction d)
{
	TerrainRangeArc range(TerrainRangeArc::Data(Point2d::Cast(from), a, c, d), RailRoad::GetTotalWidth());

	Terraformer t(range, TerraformFunctionFactory::GetConstant(from.z()));
	terrainPtr_->Apply(t);

	const RailRoadPtr rrp = RailRoadFactory::Arc(from, a, c, d);
	
	if (!roadNetwork_.Insert(rrp))
	{
		return;
	}

	staticSceneObjects_.push_back(StaticSceneObjectFactory::ForRailRoad(rrp));
}

void 
Application::EmulateDynamicScene1()
{
	const Point3d p1(10, 10, 3);
	const Point3d p2(50, 10, 3);
	
	const RailRoadPtr rrp = RailRoadFactory::Line(p1, p2);
	
	roadNetwork_.Insert(rrp);

	const RoadRoutePtr rrPtr = roadNetwork_.GetRoute(p1, p2);

	managers1_.emplace_back(&componentHolder_, RoadRouteHolder1(rrPtr, Heading::Forward));
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
	PutRailRoadArc(p2, c1, a, Direction::Right);
	PutRailRoadLine(p3, p4);
	PutRailRoadArc(p4, c2, a, Direction::Left);
	PutRailRoadLine(p5, p6);
	 
	const RoadRoutePtr rrPtr = roadNetwork_.GetRoute(p1, p6);

	roadRoutePtrs_.push_back(rrPtr);
}
