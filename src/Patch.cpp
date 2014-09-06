#include "Patch.h"
#include "Triangle3d.h"
#include "HeightMap.h"
#include "TriangleNode.h"
#include "TriangleNodeHandler.h"
#include "GeometryUtils.h"
#include "ModelData.h"
#include "Logger.h"
#include "GlobalDefines.h"
#include "PatchTriangleMapper.h"
#include <boost/range/algorithm.hpp>

using namespace trm;
using namespace trm::terrain;
using namespace trm::terrain::lod;

Patch::Patch()
	: pRootUp_(nullptr)
	, pRootDown_(nullptr)
	, detalization_(0)
	, pHeightMap_(nullptr)
{
	Reset();
}

void
Patch::Reset()
{
	delete pRootUp_;
	delete pRootDown_;

	pRootUp_ = new TriangleNode();
	pRootDown_ = new TriangleNode();
	pRootUp_->SetBase(pRootDown_);
	pRootDown_->SetBase(pRootUp_);

	rootUpCache_.ResetNode(pRootUp_);
	rootDownCache_.ResetNode(pRootDown_);
}

void
Patch::ComputeVariance()
{
	const Triangle3dPair tp = utils::GetTriangles(*pHeightMap_);

	varianceUp_.Generate(detalization_, *pHeightMap_, tp.first);
	varianceDown_.Generate(detalization_, *pHeightMap_, tp.second);
}

bool
Patch::Tasselate(const Point3d & camera)
{
	/*TriangleNodeHandler::Tasselate(pRootUp_, detalization_, varianceUp_, *pHeightMap_, UPPER_TRIANGLE_MAPPER, camera);
	TriangleNodeHandler::Tasselate(pRootDown_, detalization_, varianceDown_, *pHeightMap_, LOWER_TRIANGLE_MAPPER, camera);
	
	return false;*/

	//assert(pRootUp_->IsValid());
	//assert(pRootDown_->IsValid());

	const bool upWasUpdated = rootUpCache_.Tasselate(detalization_, varianceUp_, 
		*pHeightMap_, UPPER_TRIANGLE_MAPPER, camera);
	const bool downWasUpdate = rootDownCache_.Tasselate(detalization_, varianceDown_, 
		*pHeightMap_, LOWER_TRIANGLE_MAPPER, camera);

	return upWasUpdated || downWasUpdate;
}

void
Patch::Render(ModelData & md)
{
	const size_t sz = md.points.size();

	normaleMap_.clear();

	TriangleNodeHandler::Render(pRootUp_, detalization_, *pHeightMap_, UPPER_TRIANGLE_MAPPER, md, normaleMap_);
	TriangleNodeHandler::Render(pRootDown_, detalization_, *pHeightMap_, LOWER_TRIANGLE_MAPPER, md, normaleMap_);

	assert(pRootUp_->IsValid());
	assert(pRootDown_->IsValid());

	// we need to close a loop with adding one more triangle
	// equal to the first one
	md.points.push_back(md.points[sz]);

	md.normales.reserve(md.points.size());

	const auto normaleMapEnd = normaleMap_.end();

#ifdef DRAWING_MODE_FULL
	boost::range::for_each(md.points,
		[&](const Point3d & p)
	{
		const Size2d s = Size2d::Cast(p);
		PointNormaleMap::const_iterator found = normaleMap_.find(s);

		if (found == normaleMapEnd)
		{
			throw std::runtime_error("Unknown point found in normales map");
		}

		md.normales.push_back(found->second);
	});
#endif // DRAWING_MODE_FULL
}

Patch::~Patch()
{
	delete pRootUp_;
	delete pRootDown_;
}

void 
Patch::Init(const HeightMap & hm, const size_t det)
{
	pHeightMap_ = &hm;
	detalization_ = det;
}

void 
Patch::Attach(Patch & p, const Direction dir)
{
	switch (dir)
	{
	case Direction::Up:
		pRootUp_->SetRNeighbor(p.pRootDown_);
		p.pRootDown_->SetRNeighbor(pRootUp_);
		break;
	case Direction::Down:
		pRootDown_->SetRNeighbor(p.pRootUp_);
		p.pRootUp_->SetRNeighbor(pRootDown_);
		break;
	case Direction::Left:
		pRootUp_->SetLNeighbor(p.pRootDown_);
		p.pRootDown_->SetLNeighbor(pRootUp_);
		break;
	case Direction::Right:
		pRootDown_->SetLNeighbor(p.pRootUp_);
		p.pRootUp_->SetLNeighbor(pRootDown_);
		break;
	default:
		throw std::runtime_error("Patch::Attach(Patch & p, const Direction dir) - Incorrect direction");
	};
}

void 
Patch::Clear()
{
	normaleMap_.clear();
	varianceUp_.Clear();
	varianceDown_.Clear();
	pRootUp_->SetClearCause(false, TriangleNode::RecursiveMode::Yes);
	pRootDown_->SetClearCause(false, TriangleNode::RecursiveMode::Yes);
}

const PointNormaleMap & 
Patch::GetNormales() const
{
	return normaleMap_;
}

void
Patch::ZipNormales()
{
	const size_t sz = pHeightMap_->GetSize();

	auto it = normaleMap_.begin();
	auto end = normaleMap_.end();

	while (it != end)
	{
		const Size2d & s = it->first;

		if ((s.x() % (sz - 1) == 0) || (s.y() % (sz - 1) == 0))
		{
			++it;
		}
		else
		{
			normaleMap_.erase(it++);
		}
	}
}