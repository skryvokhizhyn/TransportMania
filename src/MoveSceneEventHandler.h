#pragma once

#include "SceneEventHandler.h"

namespace trm
{
	class Application;

	class MoveSceneEventHandler
		: public SceneEventHandler
	{
	public:
		// handler selectors
		void SetMoveHandler(Application & app);
		void SetRoadHandler(Application & app);
		void ChangeHandler(Application & app);

		// SceneEventHandler
		virtual void Commit() const override;

		// EventHandler
		virtual void Process(const FingerPressed & e) override;
		virtual void Process(const FingerMoved & e) override;
		virtual void Process(const FingerReleased & e) override;
		virtual void Reset() override;

	private:
		enum class HandlerType
		{
			Unknown,
			Move,
			Draw
		};

	private:
		HandlerType type_ = HandlerType::Unknown;
		SceneEventHandlerPtr handlerPtr_;
	};

	using MoveSceneEventHandlerPtr = std::shared_ptr<MoveSceneEventHandler>;

} // namespace trm
