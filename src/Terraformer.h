#ifndef _TERRAFORMER_H_
#define _TERRAFORMER_H_

//#include "Size.h"
#include "TerrainRange.h"
#include "TerraformFunction.h"

#include <boost/noncopyable.hpp>

#include <functional>

namespace trm
{
	struct Size2d;

namespace terrain
{
	struct HeightMapBase;

} // namespace terrain

	class Terraformer
		: boost::noncopyable
	{
	public:
		Terraformer(const TerrainRange & range, TerraformFunction & func);

		void Apply(terrain::HeightMapBase & hm);

	public:
		static bool Normalized(const Size2d & sz, TerrainRange::Range & r);

	private:
		using TerraformFunctionRef = std::reference_wrapper<TerraformFunction>;

	private:
		const TerrainRange & range_;
		TerraformFunctionRef func_;
	};

} // namespace trm

#endif // _TERRAFORMER_H_