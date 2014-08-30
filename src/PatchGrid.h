#ifndef _PATCHGRID_H_
#define _PATCHGRID_H_

#include "HeightMap.h"
#include "Patch.h"
#include "Size2d.h"
#include "HeightMapLoader.h"

#include <boost/noncopyable.hpp>
#include <boost/tuple/tuple.hpp>

#include <utility>
#include <memory>
#include <map>

namespace trm
{
	struct ModelData;
	struct Point3d;
	class WorldProjection;
	class TerrainRange;

namespace terrain
{
	class HeightMap;

namespace lod
{
	class Patch;

	class PatchGrid
		: public HeightMapBase
		, boost::noncopyable
	{
	public:
		PatchGrid(const unsigned short patchSize, const unsigned short patchCnt, const HeightMapLoaderPtr & hmlPtr);
		PatchGrid(PatchGrid && pg);
		void Init();

		void Update(const WorldProjection & wp);
		bool Tasselate(const WorldProjection & wp);
		bool Render(ModelData & md);
		void Flush();

		// HeightMapBase
		virtual size_t GetSize() const;
		virtual Type At(const Point2d & p) const;
		virtual void Set(const Point2d & p, const Type z);

	public:
		typedef std::vector<Size2d> Positions;

	public:
		Positions GetAdjucentPatches(const Point2d & p) const;

	private:
		typedef boost::tuple<HeightMap, Patch> Node;
		typedef std::shared_ptr<Node> NodePtr;
		typedef std::map<Size2d, NodePtr> GridMapType;

	private:
		static NodePtr CreateNode(const unsigned short pSz);
		static void RenderNode(const GridMapType::value_type & node, ModelData & md);
		
	private:
		void PutNode(const Size2d & pos, NodePtr nodePtr);
		void LoadHeightMap(const Size2d & pos, HeightMap & hm) const;
		void GlueNormales(const GridMapType::value_type & node, ModelData & md) const;

	private:
		const unsigned short patchSize_;
		const unsigned short patchCount_;
		GridMapType grid_;
		GridMapType::iterator currIt_;
		terrain::HeightMapLoaderPtr hmlPtr_;
	};

} // namespace lod
} // namespace terrain
} // namespace trm

#endif // _PATCHGRID_H_