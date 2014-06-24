#include "PatchTriangleMapper.h"
#include "Terrain.h"
#include "Triangle2d.h"

namespace trm
{
namespace terrain
{
	TriangleMapper UPPER_TRIANGLE_MAPPER(
		Triangle2d(
			Point2d(0, 0), 
			Point2d(0, static_cast<float>(Terrain::PATCH_SIZE - 1)), 
			Point2d(static_cast<float>(Terrain::PATCH_SIZE - 1), static_cast<float>(Terrain::PATCH_SIZE - 1))
		));

	TriangleMapper LOWER_TRIANGLE_MAPPER(
		Triangle2d(
			Point2d(
			static_cast<float>(Terrain::PATCH_SIZE - 1), static_cast<float>(Terrain::PATCH_SIZE - 1)), 
			Point2d(static_cast<float>(Terrain::PATCH_SIZE - 1), 0), 
			Point2d(0, 0)));

} // namespace terrain
} // namespace trm


