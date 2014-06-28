#include "ModelDrawerPool.h"
#include <algorithm>
#include <functional>

using namespace trm;
namespace stdp = std::placeholders;

// activates one item from a pool
// if it's empty, adds a new one
void 
ModelDrawerPool::Push(const ModelData & md)
{
	if (poolItems_.empty())
	{
		activeItems_.push_back(ModelDrawerPtr(new ModelDrawer()));
		ModelDrawerPtr & backPtr = activeItems_.back();
		backPtr->Load(md);
	}
	else
	{
		ModelDrawerPtr & topPtr = poolItems_.top();
		topPtr->Load(md);
		activeItems_.push_back(std::move(topPtr));
		poolItems_.pop();
	}
}

void 
ModelDrawerPool::DrawActive() const
{
	std::for_each(activeItems_.begin(), activeItems_.end(), std::bind(&ModelDrawer::Draw, stdp::_1));
}


// all active items are sent to pool
void 
ModelDrawerPool::Release()
{
	std::for_each(activeItems_.begin(), activeItems_.end(), [&](ModelDrawerPtr & mdPtr) {poolItems_.push(std::move(mdPtr));});
	activeItems_.clear();
}