#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "Types.h"
#include "Point3d.h"
#include <iostream>

namespace trm
{
	class Vertex
	{
	public:
		Vertex();
		Vertex(const Point3d & coord, const Point3d & normal);

		const Point3d & GetCoordinates() const;
		void SetCoordinates(const Point3d & coord);

		const Point3d & GetNormal() const;
		void SetNormal(const Point3d & normal);

	private:
		Point3d coord_;
		Point3d normal_;
	};

	std::ostream & operator << (std::ostream & o, const Vertex & v);

} // namespace trm

#endif // _VERTEX_H_