#pragma once

#include "ThreadPool.h"
#include "Matrix.h"

#include <future>

namespace trm
{
	class Terrain;
	struct Point3d;
	class WorldProjection;

	class RenderThreadedExecutor
	{
	public:
		RenderThreadedExecutor();
		~RenderThreadedExecutor();

		void Schedule();
		void Render(Terrain & t, const WorldProjection & wp);
		void Actualized();

	private:
		void UpdateState();
		bool IsScheduled() const;
		void ScheduleUpdating(Terrain & t, const Matrix & projectionViewMatrix);
		void ScheduleTasselation(Terrain & t, const Point3d & camera);

	private:
		using UpdateFuture = std::future<void>;
		using TasselateFuture = std::future<bool>;

		enum class State
		{
			Idle,
			Updating,
			Tasselating,
			Actualizing
		};

	private:
		utils::ThreadPool tp_;
		UpdateFuture updateFuture_;
		TasselateFuture tasselateFuture_;
		State state_;
		bool updateRequested_;
	};

} // namespace trm
