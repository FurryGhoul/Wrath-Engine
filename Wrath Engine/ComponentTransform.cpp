#include "ComponentTransform.h"

ComponentTransform::ComponentTransform() 
{ 
	local.SetIdentity(); 
}

ComponentTransform::ComponentTransform(Component_Type type, GameObject* parent) : Component(type, parent)
{ 
	local.SetIdentity(); 
}

ComponentTransform::~ComponentTransform() {}

math::float4x4 ComponentTransform::GetLocalMatrix()
{
	return local;
}
