#include <LeakDetectorInitializer.h>

void TerrainPerformanceTest1();
void PolygonVisibilityPerformanceCheck1();

int main(int /*argc*/, char ** /*argv*/)
{
	utils::LeakDetectorInitializer initializer;

	TerrainPerformanceTest1();
	//PolygonVisibilityPerformanceCheck1();

	return 0; 
}