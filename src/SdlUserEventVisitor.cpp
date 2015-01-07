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