#include "Vertex.h"
#include "CommonUtils.h"

using namespace trm;

Vertex::Vertex()
{
}

Vertex::Vertex(const Point3d & coord, const Point3d & normal)
	: coord_(coord), normal_(normal)
{
}

const Point3d & 
Vertex::GetCoordinates() const
{
	return coord_;
}
		
void 
Vertex::SetCoordinates(const Point3d & coord)
{
	coord_ = coord;
}

const Point3d & 
Vertex::GetNormal() const
{
	return normal_;
}

void 
Vertex::SetNormal(const Point3d & normal)
{
	normal_ = normal;
}

std::ostream & trm::operator << (std::ostream & o, const Vertex & v)
{
	o << "(Coord" << v.GetCoordinates() << "Normal" << v.GetNormal() << ")";
	
	return o;
}