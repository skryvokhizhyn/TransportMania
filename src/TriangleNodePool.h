#pragma once

#include "PoolUsageCounter.h"

#include <boost/noncopyable.hpp>
#include <boost/pool/pool.hpp>

namespace trm
{
namespace terrain
{
namespace lod
{

	class TriangleNodePool
		: boost::noncopyable
		, protected utils::PoolUsageCounterImpl
	{
	public:
		template<typename T>
		static TriangleNodePool & Get()
		{
			static TriangleNodePool instance(sizeof(T), typeid(T).name());
			
			return instance;
		}

		void * Malloc();
		void Free(void * p);

	private:
		TriangleNodePool(const std::size_t sz, std::string typeName);

	private:
		boost::pool<> pool_;
	};

} // namespace lod
} // namespace terrain
} // namespace trm
