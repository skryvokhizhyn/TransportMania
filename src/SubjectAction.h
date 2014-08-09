#pragma once

#include "Angle.h"

#include <utility>

namespace trm
{
namespace impl
{
	enum class ActionType
	{
		Undefined,
		Bend,
		Zoom,
		Rotate
	};

	struct PointMove;

	using AnglePair = std::pair<Angle, Angle>;

	extern const Angle DEFAULT_ACTION_DETECTION_ANGLE;

	ActionType GetSubjectAction(const PointMove & p1, const PointMove & p2, const Angle limitAngle);
	float GetZoomValue(const PointMove & p1, const PointMove & p2);
	Angle GetRotateAngle(const PointMove & p1, const PointMove & p2);
	AnglePair GetBendAngles(const PointMove & p1, PointMove p2);
	
} // namespace impl
} // namespace trm
