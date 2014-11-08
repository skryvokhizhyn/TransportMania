#include "TrainVisibleObject.h"
#include "MatrixFactory.h"
#include "MatrixUtils.h"
#include "ModelData.h"
#include "TrainDrawableObjectFactory.h"

using namespace trm;

namespace
{
	Matrix CalculateMatrix(const Point3d & posFrom, const Point3d & posTo)
	{
		const Matrix moveMatrix = MatrixFactory::Move(posTo);
		const Matrix rotateMatrix = MatrixFactory::Rotate(Point3d(1, 0, 0), posTo - posFrom, Point3d(0, 0, 1));

		return moveMatrix * rotateMatrix;
	}
}

TrainVisibleObject::TrainVisibleObject(const TrainPartType type, PositionWPtr posPtr, const Point3d & nextPos)
	: sourcePosition_(posPtr)
{
	ModelData md;
	auto drawable = TrainDrawableObjectFactory::Create(type);
	drawable->Render(md);

	drawer_.Load(md);

	if (auto posPtr = sourcePosition_.lock())
	{
		const Point3d & startingPos = *posPtr;
		positionFrom_ = startingPos;
		positionTo_ = nextPos;
	}
	else
	{
		throw std::runtime_error("Disposed shared position has been passed");
	}

	if (positionTo_ == positionFrom_)
	{
		throw std::runtime_error("Unable to identify moving direction. From == To");
	}
}

bool 
TrainVisibleObject::Update()
{
	if (auto posPtr = sourcePosition_.lock())
	{
		const Point3d & nextPos = *posPtr;

		// update positions when it makes sense only
		if (nextPos != positionTo_)
		{
			positionFrom_ = positionTo_;
			positionTo_ = nextPos;
		}

		return true;
	}

	return false;
}

Matrix
TrainVisibleObject::GetMatrix() const
{
	return CalculateMatrix(positionFrom_, positionTo_);
}

void 
TrainVisibleObject::Draw() const
{
	drawer_.Draw();
}
