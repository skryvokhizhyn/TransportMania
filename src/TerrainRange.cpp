#include "TerrainRange.h"
#include "Logger.h"

using namespace trm;

TerrainRange::~TerrainRange()
{}

TerrainRange::Range::Range()
	: y(0), xBegin(0), xEnd(0)
{}

TerrainRange::Range::Range(const int a, const int b, const int c)
	: y(a), xBegin(b), xEnd(c)
{}

const TerrainRange::Ranges &
TerrainRange::GetRanges() const
{
	return ranges_;
}

void 
TerrainRange::Init(const size_t sz)
{
	ranges_.reserve(sz);
}

void 
TerrainRange::PutRange(const Range & r)
{
	ranges_.push_back(r);
}

#include <iostream>

std::ostream & 
trm::operator << (std::ostream & ostr, const trm::TerrainRange::Range & r)
{
	ostr << "R(" << r.y << ", [" << r.xBegin << ", " << r.xEnd << "])";
	return ostr;
}