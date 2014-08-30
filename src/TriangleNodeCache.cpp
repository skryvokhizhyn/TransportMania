#include "TriangleNodeCache.h"
#include "TriangleNodeHandler.h"

#include <boost/range/algorithm/for_each.hpp>

using namespace trm;
using namespace trm::terrain;
using namespace trm::terrain::lod;

void
TriangleNodeCache::ResetNode(TriangleNodePtr nodePtr)
{
	nodes_.clear();
	nodes_.push_back(std::move(nodePtr));
}

bool
TriangleNodeCache::Tasselate(const size_t det, const Variance & var, const HeightMap & hm, 
	const TriangleMapper & tm, const Point3d & camera)
{
	namespace sp = std::placeholders;

	boost::for_each(nodes_,
		[&](TriangleNodePtr ptr)
	{
		if (!ptr->MarkedForDelete())
		{
			TriangleNodeHandler::TasselateLeaf(ptr, det, var, hm, tm, camera);
		}
	});

	const bool wasUpdated = Update(nodes_);
	
	if (wasUpdated)
	{
		// only leafs are stored in the cache so we can remove all children marked for delete
		boost::for_each(nodes_,	std::bind(&TriangleNode::RemoveChildren, std::placeholders::_1));
	}

	return wasUpdated;
}

bool
TriangleNodeCache::Update(TriangleNodes & nodes)
{
	bool wasUpdated = false;

	auto it = nodes.begin();
	auto end = nodes.end();

	while (it != end)
	{
		const TriangleNodePtr & ptr = *it;
		if (ptr->Splitted())
		{
			auto itLChild = nodes.insert(it, ptr->GetLChild());
			*it = ptr->GetRChild();
			it = itLChild;

			wasUpdated = true;
		}
		else if (ptr->MarkedForDelete())
		{
			TriangleNodePtr parentPtr = ptr->GetParent();
			
			if (parentPtr)
			{
				// remove current node
				*it = parentPtr;

				wasUpdated = true;
			}
		}
		else
		{
			++it;
		}
	}

	if (wasUpdated)
	{
		nodes.unique();
	}

	return wasUpdated;
}
