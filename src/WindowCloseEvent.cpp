#include "WindowCloseEvent.h"
#include "SdlUserEvent.h"
#include "UserEventData.h"

using namespace trm;
using namespace trm::UserEventData;

WindowCloseEvent::WindowCloseEvent(UniqueId id)
	: id_(id)
{}

void
WindowCloseEvent::operator() () const
{
	SdlUserEventWrapper evt = SdlUserEventCoder::Encode(CloseWindow(id_));
	evt.Emit();
}