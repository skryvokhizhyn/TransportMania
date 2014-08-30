#include <TriangleNode.h>
#include <TriangleNodeMakeShared.h>

#include <chrono>

using namespace trm::terrain::lod;
using namespace std::chrono;

void
TriangleNodePerformanceTestNew()
{
	const int count = 1000000;

	std::vector<TriangleNode*> v(count);

	steady_clock::time_point begin = steady_clock::now();

	for (int i = 0; i < count; ++i)
	{
		v.push_back(new TriangleNode());
	}

	steady_clock::time_point now = steady_clock::now();
	std::cout << "Created elements (new) " << count << " time: " << (now - begin).count() << std::endl;
	begin = now;

	while (!v.empty())
	{
		delete v.back();
		v.pop_back();
	}

	now = steady_clock::now();
	std::cout << "Deleted elements (new) " << count << " time: " << (now - begin).count() << std::endl;
	begin = now;
}

void
TriangleNodePerformanceTestStdMakeShared()
{
	using TriangleNodePtr = std::shared_ptr<TriangleNode>;

	const int count = 1000000;

	std::vector<TriangleNodePtr> v(count);

	steady_clock::time_point begin = steady_clock::now();

	for (int i = 0; i < count; ++i)
	{
		v.push_back(std::make_shared<TriangleNode>());
	}

	steady_clock::time_point now = steady_clock::now();
	std::cout << "Created elements (std::make_shared) " << count << " time: " << (now - begin).count() << std::endl;
	begin = now;

	while (!v.empty())
	{
		v.pop_back();
	}

	now = steady_clock::now();
	std::cout << "Deleted elements (std::make_shared) " << count << " time: " << (now - begin).count() << std::endl;
	begin = now;
}

void
TriangleNodePerformanceTestTrmMakeShared()
{
	const int count = 1000000;

	std::vector<TriangleNodeSPtr> v(count);

	steady_clock::time_point begin = steady_clock::now();

	for (int i = 0; i < count; ++i)
	{
		v.push_back(MakeShared());
	}

	steady_clock::time_point now = steady_clock::now();
	std::cout << "Created elements (trm::MakeShared) " << count << " time: " << (now - begin).count() << std::endl;
	begin = now;

	while (!v.empty())
	{
		v.pop_back();
	}

	now = steady_clock::now();
	std::cout << "Deleted elements (trm::MakeShared) " << count << " time: " << (now - begin).count() << std::endl;
	begin = now;
}