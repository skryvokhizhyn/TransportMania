#include "PointImpl.h"
#include "Point2d.h"
#include "TerrainRangeLine.h"
#include "GeometryUtils.h"
#include "CommonUtils.h"
#include "Line.h"
#include "RangeRectangle.h"
#include <deque>

using namespace trm;

namespace
{
	typedef std::pair<Point2d, Point2d> Point2dPair;

	Point2dPair GetLowestHighestPoints(const Point2d & p1, const Point2d & p2, const Point2d & p3, const Point2d & p4)
	{
		typedef std::vector<Point2d> Point2dVector;
		Point2dVector v;
		v.reserve(4);
		v.push_back(Point2d(p1.y(), p1.x()));
		v.push_back(Point2d(p2.y(), p2.x()));
		v.push_back(Point2d(p3.y(), p3.x()));
		v.push_back(Point2d(p4.y(), p4.x()));

		std::sort(v.begin(), v.end());

		const Point2d & invertedMin = v.front();
		const Point2d & invertedMax = v.back();

		return Point2dPair(Point2d(invertedMin.y(), invertedMin.x()), Point2d(invertedMax.y(), invertedMax.x()));
	}

	Point2d::Type GetIntersectionPoint(const Point2d::Type level, const Line & line, const bool isInfNegative)
	{
		const Line l = utils::GetLine(Point2d(0.0f, level), Point2d(1.0f, level));

		if (utils::CheckParallel(l, line))
		{
			return (isInfNegative ? -1 : 1) * std::numeric_limits<Point2d::Type>::max();
		}
		else
		{
			return utils::GetIntersectionPoint(l, line).x();
		}
	}
}

TerrainRangeLine::TerrainRangeLine(const Point2d & start, const Point2d & end, const AxisType width)
{
	assert(width >= 0.0f);

	if (start == end)
	{
		return;
	}

	/*const float d = utils::GetDistance(start, end);
	float dx = width * (end.y() - start.y()) / d / 2.0f;
	float dy = width * (end.x() - start.x()) / d / 2.0f;*/

	/*const Point2d pBD(start.x() + dx, start.y() - dy);
	const Point2d pBU(start.x() - dx, start.y() + dy);
	const Point2d pED(end.x() + dx, end.y() - dy);
	const Point2d pEU(end.x() - dx, end.y() + dy);*/
	
	/*const Line l1 = utils::GetLine(pBU, pBD);
	const Line l2 = utils::GetLine(pBD, pED);
	const Line l3 = utils::GetLine(pED, pEU);
	const Line l4 = utils::GetLine(pEU, pBU);*/

	//const RangeRectangle rr1(l1, l4, l3, l2);

	const RangeRectangle rr = GetRangeRectangle(start, end, width);
	const Point2d p1/*bu*/ = utils::GetIntersectionPoint(rr.bc, rr.ab);
	const Point2d p2/*eu*/ = utils::GetIntersectionPoint(rr.bc, rr.cd);
	const Point2d p3/*ed*/ = utils::GetIntersectionPoint(rr.da, rr.cd);
	const Point2d p4/*bd*/ = utils::GetIntersectionPoint(rr.da, rr.ab);

	//Point2dPair lowHigh1 = GetLowestHighestPoints(pBD, pBU, pED, pEU);
	Point2dPair lowHigh = GetLowestHighestPoints(p1, p2, p3, p4);
	const Point2d & lowestPoint = lowHigh.first;
	const Point2d & highestPoint = lowHigh.second;

	/*const int xMin = utils::FloatToNearInt(std::min(p1.x(), std::min(p2.x(), std::min(p3.x(), p4.x()))), &floor);
	const int xMax = utils::FloatToNearInt(std::max(p1.x(), std::max(p2.x(), std::max(p3.x(), p4.x()))), &ceil);*/
	const int xMin = utils::FloatFloorToInt(std::min(p1.x(), std::min(p2.x(), std::min(p3.x(), p4.x()))));
	const int xMax = utils::FloatCeilToInt(std::max(p1.x(), std::max(p2.x(), std::max(p3.x(), p4.x()))));

	// Ranges temp;
	std::deque<Range> temp;

	/*const int iMin = utils::FloatToNearInt(lowestPoint.y(), &floor);
	const int iMax = utils::FloatToNearInt(highestPoint.y(), &ceil);*/
	const int iMin = utils::FloatFloorToInt(lowestPoint.y());
	const int iMax = utils::FloatCeilToInt(highestPoint.y());

	assert(iMin <= iMax);

	// bottom line
	temp.push_back(Range(iMin, 
		/*utils::FloatToNearInt(lowestPoint.x(), &floor), 
		utils::FloatToNearInt(lowestPoint.x(), &ceil)*/
		utils::FloatFloorToInt(lowestPoint.x()), 
		utils::FloatCeilToInt(lowestPoint.x())
		));

	if (iMax != iMin)
	{
		boost::array<Point2d::Type, 4> points;

		for (int i = iMin + 1; i != iMax; ++i)
		{
			// to suppress conversion warnings below
			const float is = static_cast<float>(i);

			/*points[0] = GetIntersectionPoint(is, l1, true);
			points[1] = GetIntersectionPoint(is, l2, true);
			points[2] = GetIntersectionPoint(is, l3, false);
			points[3] = GetIntersectionPoint(is, l4, false);*/

			points[0] = GetIntersectionPoint(is, rr.ab, true);
			points[1] = GetIntersectionPoint(is, rr.da, true);
			points[2] = GetIntersectionPoint(is, rr.cd, false);
			points[3] = GetIntersectionPoint(is, rr.bc, false);
			
			std::sort(points.begin(), points.end());

			temp.push_back(Range(i, 
				/*utils::FloatToNearInt(points[1], &floor), 
				utils::FloatToNearInt(points[2], &ceil)*/
				utils::FloatFloorToInt(points[1]), 
				utils::FloatCeilToInt(points[2])
				));
		}

		// top line
		temp.push_back(Range(iMax, 
			/*utils::FloatToNearInt(highestPoint.x(), &floor), 
			utils::FloatToNearInt(highestPoint.x(), &ceil)*/
			utils::FloatFloorToInt(highestPoint.x()), 
			utils::FloatCeilToInt(highestPoint.x())
			));
	}

	//int ratio = utils::FloatToNearInt(utils::CheckEqual(rr.da.a, 0.0f) ? 0.0f : std::abs(rr.da.b / rr.da.a), &ceil);
	int ratio = utils::FloatCeilToInt(utils::CheckEqual(rr.da.a, 0.0f) ? 0.0f : std::abs(rr.da.b / rr.da.a));

	if (ratio == 0)
	{
		ratio = xMax - xMin;
	}

	for (Range p : temp)
	{
		const int xBegin = std::max(xMin, p.xBegin - ratio);
		const int xEnd = std::min(xMax, p.xEnd + ratio);
		
		PutRange(Range(p.y, xBegin, xEnd));
	}
} 
