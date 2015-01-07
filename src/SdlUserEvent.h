#pragma once

#include "VisitorPattern.h"

#include "SDL_events.h"

#include <memory>

namespace trm
{
	struct SdlUserEvent;

	using SdlUserEventPtr = std::unique_ptr<SdlUserEvent>;

	class SdlUserEventWrapper
		: boost::noncopyable
	{
	public:
		explicit SdlUserEventWrapper(const SDL_Event & e);
		SdlUserEventWrapper(SdlUserEventWrapper && other);
		~SdlUserEventWrapper();

		void Emit();

	private:
		SDL_Event evt_;
		bool emitted_ = false;
	};

	struct SdlUserEventCoder
	{
		template<typename Event>
		static SdlUserEventWrapper Encode(const Event & e);
		
		static SdlUserEventPtr Decode(const SDL_Event & e);
	};

	class SdlUserEventVisitor;

	struct SdlUserEvent
	{
		virtual ~SdlUserEvent() {}

		virtual void Accept(SdlUserEventVisitor & visitor) = 0;
	};

} // namespace trm
