#include "DynamicSceneObject.h"
#include "ModelData.h"
#include "MatrixFactory.h"
#include "MatrixUtils.h"

using namespace trm;

DynamicSceneObject::DynamicSceneObject(const DrawableObjectPtr & oPtr, const MovableObjectPtr & mPtr/*, const TrainWPtr & trainPtr*/)
	: mPtr_(mPtr)
	, matrix_(MatrixFactory::Identity())
{
	ModelData md;
	oPtr->Render(md);
	drawer_.Load(md);

	Update();
}

void 
DynamicSceneObject::Update()
{
	const Point3d pF = mPtr_->GetPosition();
	mPtr_->Move();
	const Point3d & pT = mPtr_->GetPosition();

	if (pT == pF)
	{
		return;
	}

	const Matrix moveMatrix = MatrixFactory::Move(pT);

	const Matrix rotateMatrix = MatrixFactory::Rotate(Point3d(1, 0, 0), pT - pF, Point3d(0, 0, 1));

	matrix_ = moveMatrix * rotateMatrix;
}

void 
DynamicSceneObject::Draw() const
{
	drawer_.Draw();
}

const Matrix & 
DynamicSceneObject::GetModelMatrix() const
{
	return matrix_;
}