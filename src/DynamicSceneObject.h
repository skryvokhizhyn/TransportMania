#pragma once

#include "ModelDrawer.h"
#include "DrawableObject.h"
#include "MovableObject.h"
#include "Train.h"
#include "Matrix.h"
#include <boost/noncopyable.hpp>
#include <memory>

namespace trm
{
	class DynamicSceneObject
		: boost::noncopyable
	{
	public:
		DynamicSceneObject(const DrawableObjectPtr & oPtr, const MovableObjectPtr & mPtr/*, const TrainWPtr & trainPtr*/);

		//bool Valid() const;
		void Update();
		void Draw() const;

		const Matrix & GetModelMatrix() const;

	private:
		ModelDrawer drawer_;
		MovableObjectPtr mPtr_;
		//TrainWPtr trainPtr_;
		Matrix matrix_;
	};

	typedef std::shared_ptr<DynamicSceneObject> DynamicSceneObjectPtr;

} // namesapce trm
