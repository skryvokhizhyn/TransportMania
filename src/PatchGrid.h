#pragma once

#include "HeightMapLoader.h"
#include "PatchGridMap.h"
#include "PointNormaleMap.h"
#include "Matrix.h"

#include <boost/noncopyable.hpp>

namespace trm
{
	struct ModelData;

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

		void Update(const Matrix & projectionViewMatrix);
		bool Tasselate(const Point3d & camera);
		void Render();
		bool GetNextRenderResult(ModelData & md);
		void Flush();

		// HeightMapBase
		virtual size_t GetSize() const;
		virtual HeightMap::Value At(const Point2d & p) const;
		virtual void Set(const Point2d & p, const Type z);

	public:
		typedef std::vector<Size2d> Positions;

	public:
		Positions GetAdjucentPatches(const Point2d & p) const;

	private:
		void LoadHeightMap(const Size2d & pos, HeightMap & hm) const;

	private:
		const unsigned short patchSize_;
		const unsigned short patchCount_;
		PatchGridMap grid_;
		PatchGridMap::SequentialIt currIt_;
		terrain::HeightMapLoaderPtr hmlPtr_;
		PointNormaleMap normaleMap_;
	};

} // namespace lod
} // namespace terrain
} // namespace trm
