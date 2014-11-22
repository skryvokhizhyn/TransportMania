#ifndef _MODELDRAWERPOOL_H_
#define _MODELDRAWERPOOL_H_

#include "ModelDrawer.h"
#include <deque>
#include <memory>

namespace trm
{
	class ModelDrawerPool
	{
	public:
		// activates one item from a pool
		// if it's empty, adds a new one
		void Push(const ModelData & md);
		void DrawActive() const;
		// all active items are sent to pool
		void Release();

	private:
		typedef std::unique_ptr<ModelDrawer> ModelDrawerPtr;
		typedef std::deque<ModelDrawerPtr> PoolType;

	private:
		PoolType poolItems_;
		PoolType activeItems_;
	};

} // namespace trm

#endif // _MODELDRAWERPOOL_H_