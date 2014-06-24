#include "MovableObject.h"

using namespace trm;

const Point3d & 
MovableObject::GetPosition() const
{
	return pos_;
}

void 
MovableObject::SetPosition(const Point3d & p)
{
	pos_ = p;
}

const Point3d & 
MovableObject::GetDirection() const
{
	return dir_;
}

void 
MovableObject::SetDirection(const Point3d & d)
{
	dir_ = d;
}