#pragma once

#include "TrainTypes.h"

namespace trm
{
	class TrainPartParameters
	{
	public:
		struct Data
		{
			Data(float a, float ms, float p, float v, float l);

			float acceleration;
			float maxSpeed;
			float power;
			float volume;
			float length;
		};

		static const Data & Get(const TrainPartType tpt);
	};

} // namespace trm
