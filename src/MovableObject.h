#pragma once

#include "Point3d.h"
#include <memory>

namespace trm
{
	class MovableObject
	{
	public:
		virtual ~MovableObject() {}

		virtual void Move() = 0;

	public:
		const Point3d & GetPosition() const;
		const Point3d & GetDirection() const;

	protected:
		void SetPosition(const Point3d & p);
		void SetDirection(const Point3d & d);

	private:
		Point3d pos_;
		Point3d dir_;
	};

	typedef std::shared_ptr<MovableObject> MovableObjectPtr;

} // namespace trm
