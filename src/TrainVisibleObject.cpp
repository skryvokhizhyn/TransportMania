#include "TrainVisibleObject.h"
#include "MatrixFactory.h"
#include "MatrixUtils.h"
#include "ModelData.h"
#include "TrainDrawableObjectFactory.h"

using namespace trm;

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
		CalculateMatrixes(startingPos, nextPos);
		position_ = startingPos;
	}
	else
	{
		throw std::runtime_error("Disposed shared position has been passed");
	}
}

bool 
TrainVisibleObject::Update()
{
	if (auto posPtr = sourcePosition_.lock())
	{
		const Point3d & nextPos = *posPtr;
		CalculateMatrixes(position_, nextPos);
		position_ = nextPos;

		return true;
	}

	return false;
}

const Matrix & 
TrainVisibleObject::GetMatrix() const
{
	return matrix_;
}

void 
TrainVisibleObject::Draw() const
{
	drawer_.Draw();
}

void 
TrainVisibleObject::CalculateMatrixes(const Point3d & from, const Point3d & to)
{
	if (from == to)
	{
		return;
	}

	const Matrix moveMatrix = MatrixFactory::Move(to);
	const Matrix rotateMatrix = MatrixFactory::Rotate(Point3d(1, 0, 0), to - from, Point3d(0, 0, 1));

	matrix_ = moveMatrix * rotateMatrix;
}
