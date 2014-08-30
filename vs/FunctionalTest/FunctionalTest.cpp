#include <PoolUsageCounterHolder.h>

// TriangleNodePerformanceTest
void TriangleNodePerformanceTestNew();
void TriangleNodePerformanceTestStdMakeShared();
void TriangleNodePerformanceTestTrmMakeShared();
// TriangleNodeLeakTest
void TriangleNodeLeakTest1();

#include <boost/multi_array.hpp>

#include <LeakDetectorInitializer.h>

int main(int /*argc*/, char ** /*argv*/)
{
	utils::LeakDetectorInitializer initializer;

	/*TriangleNodePerformanceTestNew();
	TriangleNodePerformanceTestStdMakeShared();
	TriangleNodePerformanceTestTrmMakeShared();*/

	TriangleNodeLeakTest1();

	utils::PoolUsageCounterHolder::Get().Validate();

	return 0; 
}