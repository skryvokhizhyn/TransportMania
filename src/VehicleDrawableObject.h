#pragma once

#include "DrawableObject.h"

namespace trm
{
	class VehicleDrawableObject
		: public DrawableObject
	{
	public:
		virtual void Render(ModelData & md) override;
		
		float Length() const;
		float Shift() const;
	};

} // namespace trm