#include "RenderThreadedExecutor.h"
#include "Terrain.h"
#include "ThreadPool.h"
#include "MakeUniquePtr.h"
#include "FutureTaskWrapper.h"
#include "TerrainRenderingFinishedEvent.h"
#include "WorldProjection.h"
#include "SafeExecute.h"

using namespace trm;

RenderThreadedExecutor::RenderThreadedExecutor()
	: tp_(1)
	, state_(State::Idle)
	, updateRequested_(false)
{
}

RenderThreadedExecutor::~RenderThreadedExecutor()
{
	utils::SafeExecuteWithLog([&]()
	{
		tp_.Stop();
	});
}

void 
RenderThreadedExecutor::UpdateState()
{
	if (updateFuture_.valid())
	{
		if (updateFuture_.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			updateFuture_ = UpdateFuture();
		}
	}
	else if (tasselateFuture_.valid() && state_ != State::Actualizing /* do not send actualizing twice*/)
	{
		if (tasselateFuture_.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			// update state here after finished tasselation
			state_ = State::Actualizing;

			SendTerrainRenderingFinishedEvent();
		}
	}
}

bool 
RenderThreadedExecutor::IsScheduled() const
{
	return updateFuture_.valid() || tasselateFuture_.valid();
}

void
RenderThreadedExecutor::Schedule()
{
	updateRequested_ = true;

	if (state_ == State::Idle)
	{
		updateRequested_ = false;
		state_ = State::Updating;
	}
}

void
RenderThreadedExecutor::Render(Terrain & t, const WorldProjection & wp)
{
	UpdateState();

	if (IsScheduled())
	{
		return;
	}

	switch (state_)
	{
	case State::Updating:
	{
		//utils::Logger().Trace() << "Updating";
		ScheduleUpdating(t, wp.GetProjectionViewMatrix());
	}
	break;

	case State::Tasselating:
	{
		//utils::Logger().Trace() << "Tasselating";
		ScheduleTasselation(t, wp.GetCameraPosition());
	}
	break;

	case State::Actualizing:
		//utils::Logger().Trace() << "Actualizing";
		break;

	case State::Idle:
		//utils::Logger().Trace() << "Idle";
		break;

	default:
		break;
	};
}

void 
RenderThreadedExecutor::Actualized()
{
	const bool tasselationFinished = tasselateFuture_.get();
	tasselateFuture_ = TasselateFuture();

	if (updateRequested_)
	{
		state_ = State::Updating;
		updateRequested_ = false;
	}
	else if (tasselationFinished)
	{
		state_ = State::Idle;
	}
	else
	{
		state_ = State::Tasselating;
	}
}

void 
RenderThreadedExecutor::ScheduleUpdating(Terrain & t, const Matrix & projectionViewMatrix)
{
	auto asyncUpdateFunction = [&, projectionViewMatrix]()
	{
		t.Update(projectionViewMatrix);
	};

	auto task = utils::MakeUniquePtr<FutureTaskWrapper<void>>(std::move(asyncUpdateFunction));

	updateFuture_ = task->GetFuture();

	// update state here so if multiple updates happen sequentially, we still get an update
	state_ = State::Tasselating;

	tp_.Enqueue(std::move(task));
}

void
RenderThreadedExecutor::ScheduleTasselation(Terrain & t, const Point3d & camera)
{
	auto asyncRenderFunction = [&, camera]()
	{
		const bool wasTasselated = t.Tasselate(camera);

		t.Render();

		return !wasTasselated;
	};

	auto task = utils::MakeUniquePtr<FutureTaskWrapper<bool>>(std::move(asyncRenderFunction));

	tasselateFuture_ = task->GetFuture();

	tp_.Enqueue(std::move(task));
}
