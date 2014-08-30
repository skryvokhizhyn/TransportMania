#pragma once

#include "TriangleNodeAllocator.h"
#include <memory>

namespace trm
{
namespace terrain
{
namespace lod
{

	class TriangleNode;

	template<typename... Args>
	std::shared_ptr<TriangleNode> MakeShared(Args... args)
	{
		TriangleNodeAllocator<TriangleNode> alloc;
		return std::allocate_shared<TriangleNode>(alloc, std::forward<Args>(args)...);
		//return std::make_shared<TriangleNode>(std::forward<Args>(args)...);
	}

	using TriangleNodeSPtr = std::shared_ptr<TriangleNode>;

} // namespace lod
} // namespace terrain
} // namespace trm

