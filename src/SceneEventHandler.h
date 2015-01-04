#pragma once

#include "EventHandler.h"

#include <memory>

namespace trm
{
namespace impl
{
	template<typename Subject> class EventStateMachine;

} // namespace impl

	class Application;

	class SceneEventHandler
		: public EventHandler
	{
	public:
		SceneEventHandler(Application & app);
		~SceneEventHandler();

		void Commit() const ;

		// EventHandler
		virtual void Process(const FingerPressed & e) override;
		virtual void Process(const FingerMoved & e) override;
		virtual void Process(const FingerReleased & e) override;
		virtual void Reset() override;

	private:
		std::unique_ptr<impl::EventStateMachine<Application>> eventSMPtr_;
	};

	using SceneEventHandlerPtr = std::shared_ptr<SceneEventHandler>;

} // namespace trm
