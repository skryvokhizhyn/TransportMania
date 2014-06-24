#include "TriangleNodePool.h"
#include "TriangleNode.h"

using namespace trm::terrain::lod;

namespace
{
	const size_t POOL_BLOCK_SIZE = 10000;
	const size_t POOL_CHUNKS_TO_ALLOCATE = static_cast<size_t>(POOL_BLOCK_SIZE / 2);
}

TriangleNodePool &
TriangleNodePool::Get()
{
	static TriangleNodePool instance;

	return instance;
}

TriangleNodePool::TriangleNodePool()
	: pool_(sizeof(terrain::lod::TriangleNode), POOL_CHUNKS_TO_ALLOCATE, POOL_BLOCK_SIZE)
	//, chunks_(0)
{
}

void *
TriangleNodePool::Malloc()
{
	//++chunks_;
	return pool_.malloc();
}

void
TriangleNodePool::Free(void * p)
{
	//--chunks_;
	pool_.free(p);
}