#pragma once

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
	{
	public:
		static TriangleNodePool & Get();

		void * Malloc();
		void Free(void * p);

	private:
		TriangleNodePool();

	private:
		boost::pool<> pool_;
		//int chunks_;
	};

} // namespace lod
} // namespace terrain
} // namespace trm
