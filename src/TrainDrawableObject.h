#pragma once

#include "DrawableObject.h"
#include <boost/optional.hpp>
#include <memory>

namespace trm
{
	class TrainDrawableObject
		: public DrawableObject
	{
	public:
		float GetLength() const;

	protected:
		void CalculateLength(const ModelData & md);
		void UpdateData(ModelData & md);

	private:
		typedef boost::optional<float> OptionalLength;

	private:
		OptionalLength length_;
	};

	typedef std::shared_ptr<TrainDrawableObject> TrainDrawableObjectPtr;

} // namespace trm
 