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
SdlUserEventVisitor::Visit(UserEventData::ChangeMouseMode & /*e*/)
{
	app_.ChangeMouseMode();
}

void 
SdlUserEventVisitor::Visit(UserEventData::SubmitDraftRoads & e)
{
	app_.SubmitDraftRoads(e.yesNo);
}
