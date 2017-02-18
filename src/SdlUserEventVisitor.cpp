#include "SdlUserEventVisitor.h"
#include "Application.h"

using namespace trm;
using namespace trm::UserEventData;

SdlUserEventVisitor::SdlUserEventVisitor(Application & app)
	: app_(app)
{}

void
SdlUserEventVisitor::Visit(CloseWindow & e)
{
	app_.CloseWindow(e.id);
}

void
SdlUserEventVisitor::Visit(PauseApplication & /*e*/)
{
	app_.Pause();
}

void 
SdlUserEventVisitor::Visit(UserEventData::ChangeMouseMode & e)
{
	app_.ChangeMouseMode(e.type);
}

void 
SdlUserEventVisitor::Visit(UserEventData::SubmitDraftRoads & e)
{
	app_.SubmitDraftRoads(e.yesNo);
}

void 
SdlUserEventVisitor::Visit(UserEventData::RoadAffectedIds & e)
{
	using CallbackType = void (Application::*)(UniqueId);
	CallbackType cb;

	if (e.type == RoadAffectedIds::Permanent)
		cb = &Application::DrawPermanentRailRoad;
	else
		cb = &Application::DrawTemporaryRailRoad;

	boost::for_each(e.removedIds, boost::bind(&Application::RemoveDrawnRailRoad, boost::ref(app_), _1));
	boost::for_each(e.addedIds, boost::bind(cb, boost::ref(app_), _1));
}

void
SdlUserEventVisitor::Visit(UserEventData::ActualizeTerrainRenderedData & /*e*/)
{
	app_.ActualizeRenderedData();
}