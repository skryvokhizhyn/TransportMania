#pragma once

namespace trm
{
	struct TrainMoveParameters
	{
		TrainMoveParameters()
			: maxSpeed(0.0f), acceleration(0.0f), breaking(0.0f), length(0.0f)
		{}

		float maxSpeed;
		float acceleration;
		float breaking;
		float length;
	};

} // namespace trm
