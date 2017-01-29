#include <HeightMapLoader.h>
#include <HeightMapLoaderFactory.h>
#include <Terrain.h>
#include <Settings.h>
#include <WorldProjection.h>

using namespace trm;
using namespace trm::terrain;

void TerrainPerformanceTest1()
{
	HeightMapLoaderPtr hmlPtr =
		terrain::HeightMapLoaderFactory::GetFileLoader(trm::GetHeightMapPath("hf_513.bmp"));
	HeightMapLoaderPtr hmlCachedPtr =
		terrain::HeightMapLoaderFactory::GetCachedLoader(Terrain::PATCH_SIZE, hmlPtr);

	WorldProjection wp;
	wp.SetRatio(1024, 768);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(0));
	wp.SetShift(Point3d(30, 30, 100));

	TerrainPtr terrainPtr(new Terrain(hmlCachedPtr));

	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			terrainPtr->Update(wp.GetProjectionViewMatrix());
			terrainPtr->Tasselate(wp.GetCameraPosition());
			terrainPtr->Render();

			wp.Shift(5, 0);
		}

		wp.SetShift(Point3d(30, 30 + (float)i * 5, 100));
	}
}

#include <PolygonVisibilityChecker.h>
#include <MatrixFactory.h>

void PolygonVisibilityPerformanceCheck1()
{
	Matrix pv = MatrixFactory::Identity();

	for (int i = 0; i < 1000000; ++i)
	{
		CheckPolygonIsVisible(pv,
		{
			Point3d(-2.0f, -0.1f, 10.0f), Point3d(0.0f, -2.1f, 10.0f),
			Point3d(0.0f, 2.0f, 10.0f), Point3d(-2.0f, -2.0f, 10.0f)
		});
	}
}