#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "PatchGrid.h"
#include "HeightMapLoader.h"
#include "Matrix.h"

#include <boost/noncopyable.hpp>

#include <memory>

namespace trm
{
	class Terraformer;

	class Terrain
		: private boost::noncopyable
	{
	public:
		static const size_t PATCH_SIZE;
		static const float MAX_HEIGHT;
		static const float MIN_HEIGHT;

	public:
		Terrain(const terrain::HeightMapLoaderPtr & mdl);
		Terrain(Terrain && t);

		void Update(const Matrix & projectionViewMatrix);
		bool Tasselate(const Point3d & camera);
		void Render();
		bool GetNextRenderResult(ModelData & md);
		
		void Apply(Terraformer & t);

	private:
		terrain::lod::PatchGrid patchGrid_;
	};

	typedef std::shared_ptr<Terrain> TerrainPtr;

} // namespace trm

#endif // _TERRAIN_H_
