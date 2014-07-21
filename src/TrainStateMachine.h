#pragma once 

#pragma warning(push)
#pragma warning(disable: 4512 4127)

// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>

namespace trm
{
namespace impl
{
	namespace bmf = boost::msm::front;

	template<typename TransportManagerType>
	class TrainStateMachine
	{
	public:
		// main process loop
		void Update(TransportManagerType * pTm);

	private:
		// Events
		struct Inited {};
		struct Loaded {};
		struct Stopped {};
		struct Unloaded {};

		class TrainState
		{
		public:
			virtual ~TrainState() {}
			// requires non-abstract class
			virtual void Process(TransportManagerType * /*pTM*/) 
			{
				// must never be called
				assert(false);
			}
		};

		// forward for back-end declaration
		struct TrainSMImpl;

		// Pick a back-end
		typedef boost::msm::back::state_machine<TrainSMImpl> TrainSM;

		struct TrainSMImpl : public boost::msm::front::state_machine_def<TrainSMImpl, TrainState>
		{
			class TrainSMHolder : public bmf::state<TrainState, bmf::sm_ptr>
			{
			public:
				TrainSMHolder() : pSM_(nullptr) {}

				void set_sm_ptr(TrainSM * pSM) { pSM_ = pSM; }

			protected:
				TrainSM * GetStateMachinePtr() const { return pSM_; }

			private:
				TrainSM * pSM_;
			};

			// states
			struct InitingState : public TrainSMHolder
			{
				virtual void Process(TransportManagerType * pTM) override;
			};

			struct LoadingState : public TrainSMHolder
			{
				virtual void Process(TransportManagerType * pTM) override;
			};

			struct MovingState : public TrainSMHolder
			{
				virtual void Process(TransportManagerType * pTM) override;
			};

			struct UnloadingState : public TrainSMHolder
			{
				virtual void Process(TransportManagerType * pTM) override;
			};

			//typedef EmptyState initial_state;
			typedef InitingState initial_state;

			struct transition_table : boost::mpl::vector<
				//			Start			Event       Next			Action		Guard
				bmf::Row<	InitingState,	Inited,		LoadingState,	bmf::none,	bmf::none >,
				bmf::Row<	LoadingState,	Loaded,		MovingState,	bmf::none,	bmf::none >,
				bmf::Row<	MovingState,	Stopped,	UnloadingState,	bmf::none,	bmf::none >,
				bmf::Row<	UnloadingState,	Unloaded,	InitingState,	bmf::none,	bmf::none >
				> {};

			// Replaces the default no-transition response.
			template <class FSM, class Event>
			void no_transition(const Event & /*e*/, FSM &, int state)
			{
				std::stringstream ss;
				ss << "no transition from state " << state
					<< " on event " << typeid(Event).name();

				throw std::runtime_error(ss.str());
			}
		};

	private:
		TrainSM trainStateMachine_;
	};

	template<typename TransportManagerType>
	void TrainStateMachine<TransportManagerType>::Update(TransportManagerType * pTm)
	{
		const int * currentStateId = trainStateMachine_.current_state();
		TrainState * pTS = trainStateMachine_.get_state_by_id(*currentStateId);
		pTS->Process(pTm);
	}

	template<typename TransportManagerType>
	void TrainStateMachine<TransportManagerType>::TrainSMImpl::InitingState::Process(TransportManagerType * pTM)
	{
		if (pTM->Init())
		{
			this->GetStateMachinePtr()->process_event(Inited());
		}
	}

	template<typename TransportManagerType>
	void TrainStateMachine<TransportManagerType>::TrainSMImpl::LoadingState::Process(TransportManagerType * pTM)
	{
		if (pTM->Load())
		{
			this->GetStateMachinePtr()->process_event(Loaded());
		}
	}

	template<typename TransportManagerType>
	void TrainStateMachine<TransportManagerType>::TrainSMImpl::MovingState::Process(TransportManagerType * pTM)
	{
		if (pTM->Move())
		{
			this->GetStateMachinePtr()->process_event(Stopped());
		}
	}

	template<typename TransportManagerType>
	void TrainStateMachine<TransportManagerType>::TrainSMImpl::UnloadingState::Process(TransportManagerType * pTM)
	{
		if (pTM->Unload())
		{
			this->GetStateMachinePtr()->process_event(Unloaded());
		}
	}

} // namespace impl
} // namespace trm

#pragma warning(pop)
