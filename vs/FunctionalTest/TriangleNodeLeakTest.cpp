#include <TriangleNode.h>
#include <TriangleNodeMakeShared.h>

#include "LeakDetectorNew.h"

using namespace trm::terrain::lod;

void
TriangleNodeLeakTest1()
{
	TriangleNodeSPtr tn = MakeShared();
	tn->Split();

	TriangleNode * ptr = new TriangleNode();
	delete ptr;
}