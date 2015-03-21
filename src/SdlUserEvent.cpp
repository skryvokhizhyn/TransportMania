#include "SdlUserEvent.h"
#include "UserEventData.h"
#include "SdlUserEventVisitor.h"

#include <boost/format.hpp>

#include <map>
#include <cassert>
#include <type_traits>

using namespace trm;
using namespace UserEventData;

namespace priv
{
	enum SdlUserEventType : unsigned short;

	struct UserEventType
	{
		template<typename T>
		SdlUserEventType Get();
	};

	template<typename Event>
	class SdlUserEventTmpl
		: public SdlUserEvent
	{
	public:
		SdlUserEventTmpl(const Event & evt)
			: event_(evt)
		{}

		virtual void Accept(SdlUserEventVisitor & visitor) override
		{
			visitor.Visit(event_);
		}

	private:
		Event event_;
	};

	template<typename Event>
	SdlUserEventPtr ConvertAndClear(void * p)
	{
		using EventPtr = std::unique_ptr<Event>;

		EventPtr tmpEventPtr(static_cast<Event*>(p));

		return SdlUserEventPtr(new SdlUserEventTmpl<Event>(*tmpEventPtr.get()));
	}
}

using namespace priv;

template<typename Event>
SdlUserEventWrapper 
SdlUserEventCoder::Encode(const Event & e)
{
	SDL_Event evt;
	evt.type = SDL_USEREVENT;
	evt.user.code = static_cast<int>(UserEventType().Get<Event>());
	evt.user.data1 = new Event(e);
	evt.user.data2 = nullptr;

	return SdlUserEventWrapper(evt);
}

SdlUserEventWrapper::SdlUserEventWrapper(const SDL_Event & e)
	: evt_(e)
{}

SdlUserEventWrapper::SdlUserEventWrapper(SdlUserEventWrapper && other)
	: evt_(other.evt_)
{
	SDL_Event evt;
	SDL_zero(evt);
	// clear event data
	other.evt_ = evt;
	// set up the flag to avoid deletion
	other.emitted_ = true;
}

SdlUserEventWrapper::~SdlUserEventWrapper()
{
	if (!emitted_)
	{
		// ok to call with overhead as this MUST NOT happen
		// it will identify the type and free memory correctly
		SdlUserEventCoder::Decode(evt_);
	}
}

void
SdlUserEventWrapper::Emit()
{
	SDL_PushEvent(&evt_);
	emitted_ = true;
}

#define USER_EVENT_TO_ENUM_TYPE(cls, typ) \
	template<> SdlUserEventType UserEventType::Get< cls >() { return SdlUserEventType:: typ ; }
	
#define USER_EVENT_ENCODE_SPECIALIZATION(cls) \
	template SdlUserEventWrapper SdlUserEventCoder::Encode< cls >(std::add_const< cls >::type &)

#define USER_EVENT_DECODE_SWITCH_CASE(typ,cls) \
	case SdlUserEventType:: typ : \
		return ConvertAndClear< cls >(e.user.data1)

namespace priv
{
	enum SdlUserEventType : unsigned short
	{
		CloseWindowType,
		PauseApplicationType,
		MouseModeChangeType,
		SubmitDraftRoadsType
	};

	// specialize for every user event data
	USER_EVENT_TO_ENUM_TYPE(CloseWindow, CloseWindowType);
	USER_EVENT_TO_ENUM_TYPE(PauseApplication, PauseApplicationType);
	USER_EVENT_TO_ENUM_TYPE(ChangeMouseMode, MouseModeChangeType);
	USER_EVENT_TO_ENUM_TYPE(SubmitDraftRoads, SubmitDraftRoadsType);

} // namespace priv

USER_EVENT_ENCODE_SPECIALIZATION(CloseWindow);
USER_EVENT_ENCODE_SPECIALIZATION(PauseApplication);
USER_EVENT_ENCODE_SPECIALIZATION(ChangeMouseMode);
USER_EVENT_ENCODE_SPECIALIZATION(SubmitDraftRoads);

SdlUserEventPtr 
SdlUserEventCoder::Decode(const SDL_Event & e)
{
	assert(e.type == SDL_USEREVENT);
	
	switch (e.user.code)
	{
		USER_EVENT_DECODE_SWITCH_CASE(CloseWindowType, CloseWindow);
		USER_EVENT_DECODE_SWITCH_CASE(PauseApplicationType, PauseApplication);
		USER_EVENT_DECODE_SWITCH_CASE(MouseModeChangeType, ChangeMouseMode);
		USER_EVENT_DECODE_SWITCH_CASE(SubmitDraftRoadsType, SubmitDraftRoads);

	default:
		throw std::runtime_error((boost::format("Unknown User Event id=%d given for decoding") % e.user.code).str());
	}
}

#undef USER_EVENT_DECODE_SWITCH_CASE
#undef USER_EVENT_TO_ENUM_TYPE
#undef USER_EVENT_ENCODE_SPECIALIZATION
