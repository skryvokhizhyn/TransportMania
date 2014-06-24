#ifndef _MODELDRAWERPOOL_H_
#define _MODELDRAWERPOOL_H_

#include "ModelDrawer.h"
#include <vector>
#include <stack>
#include <memory>

namespace trm
{
	class ModelDrawerPool
	{
		typedef std::unique_ptr<ModelDrawer> ModelDrawerPtr;
		typedef std::stack<ModelDrawerPtr> PoolType;
		typedef std::vector<ModelDrawerPtr> ActiveType;

	public:
		// activates one item from a pool
		// if it's empty, adds a new one
		void Push(const ModelData & md);
		void DrawActive() const;
		// all active items are sent to pool
		void Release();

	private:
		PoolType poolItems_;
		ActiveType activeItems_;
	};

} // namespace trm

#endif // _MODELDRAWERPOOL_H_