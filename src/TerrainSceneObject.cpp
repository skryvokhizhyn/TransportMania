#include "TerrainSceneObject.h"
#include "MatrixFactory.h"
#include "ModelData.h"
#include "Logger.h"
#include "Point2d.h"
#include "Point3d.h"
#include "WorldProjection.h"

using namespace trm;

TerrainSceneObject::TerrainSceneObject(const TerrainPtr & terrainPtr)
	: terrainPtr_(terrainPtr)
{
	UpdateRequired();
}

void
TerrainSceneObject::UpdateRequired()
{
	renderer_.Schedule();
}

void 
TerrainSceneObject::Render(const WorldProjection & wp)
{
	renderer_.Render(*terrainPtr_, wp);
}

void 
TerrainSceneObject::Actualize()
{
	modelDrawerPool_.Release();

	ModelData modelData;
	while (terrainPtr_->GetNextRenderResult(modelData))
	{
		if (!modelData.Valid())
		{
			continue;
		}

		modelDrawerPool_.Push(modelData);

		modelData.Clear();
	}

	renderer_.Actualized();
}

void 
TerrainSceneObject::Draw() const
{
	modelDrawerPool_.DrawActive();
}

const Matrix &
TerrainSceneObject::GetModelMatrix() const
{
	static Matrix modelMatrix = MatrixFactory::Identity();
	return modelMatrix;
}