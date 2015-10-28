#pragma once

#include "SceneEventHandler.h"
#include "MoveSceneEventHandlerType.h"

#include <boost/noncopyable.hpp>

namespace trm
{
	class Application;

	class MoveSceneEventHandler
		: public SceneEventHandler
		, boost::noncopyable
	{
	public:
		MoveSceneEventHandler(Application & app);

		//void ChangeHandler(Application & app);
		void ChangeHandler(MoveSceneEventHandlerType type);

		// SceneEventHandler
		virtual void Commit() const override;

		// EventHandler
		virtual void Process(const FingerPressed & e) override;
		virtual void Process(const FingerMoved & e) override;
		virtual void Process(const FingerReleased & e) override;
		virtual void Reset() override;

	private:
		void SetMoveHandler();
		void SetRoadHandler();

	private:
		//HandlerType type_ = HandlerType::Unknown;
		Application & appRef_;
		SceneEventHandlerPtr handlerPtr_;
	};

	using MoveSceneEventHandlerPtr = std::shared_ptr<MoveSceneEventHandler>;

} // namespace trm
