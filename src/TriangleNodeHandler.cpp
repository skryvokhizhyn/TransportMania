#include "TriangleNodeHandler.h"
#include "Point3d.h"
#include "Variance.h"
#include "HeightMap.h"
#include "GeometryUtils.h"
#include "Logger.h"
#include "ModelData.h"
#include "GlobalDefines.h"
#include "TriangleMapper.h"
#include "PointNormaleMap.h"

using namespace trm;
using namespace trm::terrain;
using namespace trm::terrain::lod;

namespace
{
	float GetWeight(const float dist)
	{
		const static float norm = 200.0f;
		const float val = dist / norm;

		return val * val;
	}

	void PutNormales(const Triangle3d & t, PointNormaleMap & normaleMap)
	{
		const Point3d normale = utils::GetNormaleForTriangleNonNormalized(t);

		normaleMap.Put(Size2d::Cast(t.l()), normale);
		normaleMap.Put(Size2d::Cast(t.e()), normale);
		normaleMap.Put(Size2d::Cast(t.r()), normale);
	}
}

void 
TriangleNodeHandler::Tasselate(TriangleNode * pTn, const size_t level, const Variance & var,
	const HeightMap & hm, const TriangleMapper & tm, const Point3d & camera)
{
	assert(pTn);

	if (TasselateImpl(pTn, level, var, hm, tm, camera))
	{
		Tasselate(pTn->GetLChild(), level, var, hm, tm, camera);
		Tasselate(pTn->GetRChild(), level, var, hm, tm, camera);
	}
}

bool 
TriangleNodeHandler::TasselateImpl(TriangleNode * pTn, const size_t det, const Variance & var, const HeightMap & hm,
	const TriangleMapper & tm, const Point3d & camera)
{
	assert(pTn);

	const size_t num = pTn->GetNum();

	if (utils::GetPowerOf2<false>(num) >= det)
	{
		return false;
	}

	const Point2d edge = tm.GetTriangleByNumber(num * 2).e();
	Point3d t = Point3d::Cast(edge);
	t.z() = hm.At(edge);

	if (!ShouldContinue(var, num, camera, t))
	{
		// all children automatically cleared
		pTn->SetClearCause(false, TriangleNode::RecursiveMode::Yes);

		pTn->Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Mark);

		return false;
	}
	else
	{
		pTn->SetClearCause(true, TriangleNode::RecursiveMode::No);

		pTn->Split();

		assert(pTn->Splitted());

		return true;
	}

	//return false;
}

void 
TriangleNodeHandler::TasselateLeaf(TriangleNode * pTn, const size_t det, const Variance & var, const HeightMap & hm,
	const TriangleMapper & tm, const Point3d & camera)
{
	assert(pTn);

	const size_t num = pTn->GetNum();

	const Triangle2d & tr = tm.GetTriangleByNumber(num * 2);

	// merge when we are at the deepest level
	bool shouldMerge = (utils::GetPowerOf2<false>(num) == det);
	// if not at the deepest then check variance
	if (!shouldMerge)
	{
		Point3d t = Point3d::Cast(tr.e());
		t.z() = hm.At(tr.e());
		shouldMerge = !ShouldContinue(var, num, camera, t);
	}

	if (shouldMerge)
	{
		TriangleNode * pParent = pTn->GetParent();

		if (pParent)
		{
			Point3d p = Point3d::Cast(tr.l());
			p.z() = hm.At(tr.l());

			if (!ShouldContinue(var, pParent->GetNum(), camera, p))
			{
				// all children automatically cleared
				pParent->SetClearCause(false, TriangleNode::RecursiveMode::No);

				pParent->Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Mark);
			}
		}
	}
	else
	{
		pTn->SetClearCause(true, TriangleNode::RecursiveMode::No);

		pTn->Split();

		assert(pTn->Splitted());
	}
}

bool 
TriangleNodeHandler::ShouldContinue(const Variance & var, const size_t num, 
	const Point3d & camera, const Point3d & t)
{
	const float distToCenter = utils::GetDistance(camera, t);
	const float variance = var.At(num - 1);
	 
	if (variance < GetWeight(distToCenter))
		return false;

	return true;
}

void 
TriangleNodeHandler::Render(TriangleNode * pTn, const size_t level, const HeightMap & hm, const TriangleMapper & tm, 
	ModelData & md, PointNormaleMap & normaleMap)
{
	assert(pTn);

	const bool even = (level % 2 == 0);

	if (pTn->Splitted())
	{
		if (even)
		{
			Render(pTn->GetLChild(), level + 1, hm, tm, md, normaleMap);
			Render(pTn->GetRChild(), level + 1, hm, tm, md, normaleMap);
		}
		else
		{
			Render(pTn->GetRChild(), level + 1, hm, tm, md, normaleMap);
			Render(pTn->GetLChild(), level + 1, hm, tm, md, normaleMap);
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
		PutNormales(t3d, normaleMap);
		
		assert(md.type == ModelData::Mode::Undefined || md.type == ModelData::Mode::Triangle);
		md.type = ModelData::Mode::Triangle;
#else
		assert(md.type == ModelData::Mode::Undefined || md.type == ModelData::Mode::Line);
		md.type = ModelData::Mode::Line;
#endif // DRAWING_MODE_FULL
	}

}