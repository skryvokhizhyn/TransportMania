#include "TriangleNodePool.h"
#include "TriangleNode.h"

using namespace trm::terrain::lod;

#include <boost/pool/pool.hpp>

namespace
{
	const size_t POOL_BLOCK_SIZE = 10000;
	const size_t POOL_CHUNKS_TO_ALLOCATE = static_cast<size_t>(POOL_BLOCK_SIZE / 2);
}

TriangleNodePool::TriangleNodePool(const std::size_t sz, std::string typeName)
	: utils::PoolUsageCounterImpl(typeName)
	, pool_(sz, POOL_CHUNKS_TO_ALLOCATE, POOL_BLOCK_SIZE)
{
}

#undef malloc
#undef free

void *
TriangleNodePool::Malloc()
{
	Inc();
	return pool_.malloc();
}

void
TriangleNodePool::Free(void * p)
{
	Dec();
	pool_.free(p);
}