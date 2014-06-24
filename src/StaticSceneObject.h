#pragma once

#include "ModelDrawer.h"
#include "DrawableObject.h"
#include <boost/noncopyable.hpp>
#include <memory>

namespace trm
{
	class StaticSceneObject
		: boost::noncopyable
	{
	public:
		StaticSceneObject(const DrawableObjectPtr & oPtr);
		
		void Draw() const;

	private:
		ModelDrawer drawer_;
	};

	typedef std::shared_ptr<StaticSceneObject> StaticSceneObjectPtr;

} // namespace trm