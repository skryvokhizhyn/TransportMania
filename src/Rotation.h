#pragma once

#include <iostream>

namespace trm
{
	enum class Rotation
	{
		Clockwise,
		AntiClockwise
	};

	inline std::ostream & operator << (std::ostream & o, const Rotation r)
	{
		switch (r)
		{
		case Rotation::Clockwise:
			o << "Clockwise";
			break;

		case Rotation::AntiClockwise:
			o << "AntiClockwise";
			break;
		}

		return o;
	}

} // namespace trm
