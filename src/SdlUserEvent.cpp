#include "SdlUserEvent.h"
#include "UserEventData.h"
#include "SdlUserEventVisitor.h"

#include <boost/format.hpp>

#include <map>
#include <cassert>

using namespace trm;
using namespace UserEventData;

namespace
{
	enum SdlUserEventType
	{
		CloseWindowType
	};

	struct UserEventType
	{
		template<typename T>
		SdlUserEventType Get();
	};

	// specialize for every user event data
	template<> SdlUserEventType UserEventType::Get<CloseWindow>() { return SdlUserEventType::CloseWindowType; }

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

template SdlUserEventWrapper SdlUserEventCoder::Encode<CloseWindow>(const CloseWindow &);

SdlUserEventPtr 
SdlUserEventCoder::Decode(const SDL_Event & e)
{
	assert(e.type == SDL_USEREVENT);
	
	switch (e.user.code)
	{
	case SdlUserEventType::CloseWindowType:
		return ConvertAndClear<CloseWindow>(e.user.data1);

	default:
		throw std::runtime_error((boost::format("Unknown User Event id=%d given for decoding") % e.user.code).str());
	}
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
