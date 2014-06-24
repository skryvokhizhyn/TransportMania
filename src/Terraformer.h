#ifndef _TERRAFORMER_H_
#define _TERRAFORMER_H_

//#include "Size.h"
#include "TerrainRange.h"
#include "TerraformFunction.h"
#include <boost/noncopyable.hpp>

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
		Terraformer(const TerrainRange & range, TerraformFunction func);

		void Apply(terrain::HeightMapBase & hm);
		//const TerrainRange & GetRange() const;

	public:
		static bool Normalized(const Size2d & sz, TerrainRange::Range & r);

	private:
		const TerrainRange & range_;
		TerraformFunction func_;
	};

} // namespace trm

#endif // _TERRAFORMER_H_