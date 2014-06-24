#include "StaticSceneObject.h"
#include "ModelData.h"
#include "Point3d.h"

using namespace trm;

StaticSceneObject::StaticSceneObject(const DrawableObjectPtr & oPtr)
{
	ModelData md;
	oPtr->Render(md);
	drawer_.Load(md);
}

void
StaticSceneObject::Draw() const
{
	drawer_.Draw();
}