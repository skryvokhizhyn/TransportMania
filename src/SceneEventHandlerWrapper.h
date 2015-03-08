#pragma once

#include "SceneEventHandler.h"
#include "EventStateMachineDefs.h"

namespace trm
{
	template<typename T>
	class SceneEventHandlerWrapper
		: public SceneEventHandler
	{
	public:
		SceneEventHandlerWrapper(T * pImpl)
			: implPtr_(pImpl)
		{}

		virtual void Commit() const override { implPtr_->Emit(impl::Commit()); }

		virtual void Process(const FingerPressed & e) override { implPtr_->Emit(e); }
		virtual void Process(const FingerReleased & e) override { implPtr_->Emit(e); }
		virtual void Process(const FingerMoved & e) override { implPtr_->Emit(e); }

		virtual void Reset() override { implPtr_->Emit(impl::Reset()); }

	private:
		using ImplPtr = std::unique_ptr<T>;

	private:
		ImplPtr implPtr_;
	};

	template<typename T>
	SceneEventHandlerPtr MakeSceneWrapper(T * t)
	{
		return SceneEventHandlerPtr(new SceneEventHandlerWrapper<T>(t));
	}

} // namespace trm
