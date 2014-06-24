#include "TriangleNodeHandler.h"
#include "Point3d.h"
#include "Variance.h"
#include "HeightMap.h"
#include "TriangleNode.h"
#include "GeometryUtils.h"
#include "Logger.h"
#include "Point4d.h"
#include "MatrixUtils.h"
#include "ModelData.h"
#include "GlobalDefines.h"
#include "TriangleMapper.h"
#include "Triangle3d.h"

using namespace trm;
using namespace trm::terrain;
using namespace trm::terrain::lod;

void 
TriangleNodeHandler::Tasselate(TriangleNode * pTn, const size_t level, const Variance & var,
	const HeightMap & hm, const TriangleMapper & tm, /*const Triangle3d & t,*/ const Point3d & camera)
{
	assert(pTn != nullptr);

	// ignore level 0 nodes
	if (level == 0)
	//if (level < 5)
	{
		return;
	}

	//const Triangle3dPair tp = utils::SplitTriangle(t, hm);
	const Point2d edge = tm.GetTriangleByNumber(pTn->GetNum() * 2).e();
	Point3d t = Point3d::Cast(edge);
	t.z() = hm.At(edge);

	if (!ShouldContinue(var, pTn->GetNum(), camera, /*tp.first*/t))
	{
		pTn->Merge(TriangleNode::ProcessCause::Clear, TriangleNode::ProcessBase::Merge);
	}
	else
	{
		pTn->SetClearCause(true, false);

		pTn->Split();

		assert(pTn->Splitted());

		Tasselate(pTn->GetLChild(), level - 1, var, hm, tm, /*tp.first, */camera);
		Tasselate(pTn->GetRChild(), level - 1, var, hm, tm, /*tp.second, */camera);
	}
}

namespace
{
	float GetWeight(const float dist)
	{
		const static float norm = 200.0f;
		const float val = dist / norm;

		return val * val;
	}

	void PutPointNormale(const Point3d & p, const Point3d & normale, PointNormaleMap & normaleMap)
	{
		const Size2d s = Size2d::Cast(p);

		const auto found = normaleMap.lower_bound(s);
		if (found != normaleMap.end() && !PointNormaleMap::key_compare()(s, found->first))
		{
			found->second += normale;
		}
		else
		{
			normaleMap.insert(found, std::make_pair(s, normale));
		}
	}

	void PutNormales(const Triangle3d & t, PointNormaleMap & normaleMap)
	{
		const Point3d normale = utils::GetNormaleForTriangleNonNormalized(t);

		PutPointNormale(t.l(), normale, normaleMap);
		PutPointNormale(t.e(), normale, normaleMap);
		PutPointNormale(t.r(), normale, normaleMap);
	}
}

bool 
TriangleNodeHandler::ShouldContinue(const Variance & var, const size_t num, 
	const Point3d & camera, /*const Triangle3d & t*/ const Point3d & t)
{
	//const float distToCenter = utils::GetDistance(camera, t.e()/*sphere center*/);
	const float distToCenter = utils::GetDistance(camera, t/*sphere center*/);
	//const float sphereRadii = utils::GetDistance(t.e(), t.l());
	const float variance = var.At(num - 1);
	 
	//const float weight = 0.001f;

	//const float dist = distToCenter > sphereRadii ? distToCenter - sphereRadii : distToCenter / sphereRadii;
	//const float ratio = variance / dist;
	//const float ratio = variance / distToCenter;

	//static float maxHeight = 0;
	//if (maxHeight < distToCenter)
	//{
	//	maxHeight = distToCenter;
	//	utils::Logger().Debug() << "max " << maxHeight;
	//}
	//static float minHeight = 1000;
	//if (minHeight > distToCenter)
	//{
	//	minHeight = distToCenter;
	//	utils::Logger().Debug() << "min " << minHeight;
	//}

	//if (ratio < weight)
	if (variance < GetWeight(distToCenter))
		return false;

	return true;
}

void 
TriangleNodeHandler::Render(TriangleNode * pTn, const size_t level, const HeightMap & hm, const TriangleMapper & tm, /*const Triangle3d & t, */
	ModelData & md, PointNormaleMap & normaleMap)
{
	const bool even = (level % 2 == 0);

	if (pTn->Splitted())
	{
		//const Triangle3dPair tp = utils::SplitTriangle(t, hm);

		if (even)
		{
			Render(pTn->GetLChild(), level + 1, hm, tm, /*tp.first, */md, normaleMap);
			Render(pTn->GetRChild(), level + 1, hm, tm, /*tp.second, */md, normaleMap);
		}
		else
		{
			Render(pTn->GetRChild(), level + 1, hm, tm, /*tp.second, */md, normaleMap);
			Render(pTn->GetLChild(), level + 1, hm, tm, /*tp.first, */md, normaleMap);
		}
	}
	else
	{
		// should be before pushing values to it
		size_t sz = md.points.size();

		const Triangle2d & t2d = tm.GetTriangleByNumber(pTn->GetNum());
		const Triangle3d t3d = utils::GetTriangle3dFrom2d(t2d, hm);

		if (even)
		{
			/*md.points.push_back(t.l());
			md.points.push_back(t.e());*/

			md.points.push_back(t3d.l());
			md.points.push_back(t3d.e());

#ifdef DRAWING_MODE_FULL
			// for triangles
			sz += 2;
			md.indexes.push_back(sz);
			md.indexes.push_back(--sz);
			md.indexes.push_back(--sz);
#endif // DRAWING_MODE_FULL
		}
		else
		{
			/*md.points.push_back(t.r());
			md.points.push_back(t.e());*/
			md.points.push_back(t3d.r());
			md.points.push_back(t3d.e());

#ifdef DRAWING_MODE_FULL
			// for triangles
			md.indexes.push_back(sz++);
			md.indexes.push_back(sz++);
			md.indexes.push_back(sz);
#endif // DRAWING_MODE_FULL
		}

		
#ifndef DRAWING_MODE_FULL
		// for lines
		md.indexes.push_back(sz++);
		md.indexes.push_back(sz);
		md.indexes.push_back(sz++);
		md.indexes.push_back(sz);
		md.indexes.push_back(sz - 2);
		md.indexes.push_back(sz);
#endif // DRAWING_MODE_FULL

#ifdef DRAWING_MODE_FULL
		//PutNormales(t, normaleMap);
		PutNormales(t3d, normaleMap);
#endif // DRAWING_MODE_FULL
	}

}