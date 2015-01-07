#include "WindowPosition.h"

using namespace trm;

namespace
{
	float GetPos(size_t maxSize, size_t currSize, WindowPosition pos)
	{
		const float m = boost::numeric_cast<float>(maxSize);
		const float c = boost::numeric_cast<float>(currSize);

		switch (pos)
		{
		case WindowPosition::p0:
			return 0.0f;
		case WindowPosition::p100:
			return std::max(m - c, 0.0f);
		case WindowPosition::p25:
			{
				const float mid = c / 2.0f;
				const float glob = m / 4.0f;
				return std::max<float>(glob - mid, 0.0f);
			}
		case WindowPosition::p50:
			{
				const float mid = c / 2.0f;
				const float glob = m / 2.0f;
				return std::max<float>(glob - mid, 0.0f);
			}
		case WindowPosition::p75:
			{
				const float mid = c / 2.0f;
				const float glob = m / 4.0f * 3.0f;
				if (glob + mid > m)
				{
					return m - c;
				}
				return std::max<float>(glob - mid, 0.0f);
			}
		default:
			throw std::runtime_error((boost::format("Unknown Window Position %d has been given") % static_cast<int>(pos)).str());
		}
	}
}

trm::Point2d
trm::GetWindowPosition(const Size2d & screenSize, const Size2d & windowSize, 
		WindowPosition horizontalPos, WindowPosition verticalPos)
{
	const float x = GetPos(screenSize.x(), windowSize.x(), horizontalPos);
	const float y = GetPos(screenSize.y(), windowSize.y(), verticalPos);

	return Point2d(x, y);
}