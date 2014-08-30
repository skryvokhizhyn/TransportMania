#ifndef _TERRAINSCENEOBJECT_H_
#define _TERRAINSCENEOBJECT_H_

#include "ModelDrawerPool.h"
#include "Terrain.h"
#include "Matrix.h"
#include <boost/noncopyable.hpp>
#include <memory>

namespace trm
{
	class WorldProjection;
	class DrawContext;

	class TerrainSceneObject
		: boost::noncopyable
	{
	public:
		TerrainSceneObject(const TerrainPtr & terrain);

		void Update(const WorldProjection & wp);
		void Render(const WorldProjection & wp);
		void Draw() const;

		void UpdateRequired();

		const Matrix & GetModelMatrix() const;

	private:
		ModelDrawerPool modelDrawerPool_;
		TerrainPtr terrainPtr_;
		Matrix modelMatrix_;
		bool tasselated_ = false;
		bool updated_ = false;
	};

	typedef std::shared_ptr<TerrainSceneObject> TerrainSceneObjectPtr;

} // namespace trm

#endif //_TERRAINSCENEOBJECT_H_