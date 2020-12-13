#include "ComponentTransform.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform() 
{ 
	compTranslation = float3::zero;
	compScale = float3::one;
	compRotation = Quat::identity;

	localMatrix = float4x4::identity;
	globalMatrix = float4x4::identity;
}

ComponentTransform::ComponentTransform(Component_Type type, GameObject* parent) : Component(type, parent)
{ 
	compTranslation = float3::zero;
	compScale = float3::one;
	compRotation = Quat::identity;

	localMatrix = float4x4::FromTRS(compTranslation, compRotation, compScale);
	globalMatrix = localMatrix;
}

ComponentTransform::~ComponentTransform() {}

math::float4x4 ComponentTransform::GetLocalMatrix()
{
	return localMatrix;
}

void ComponentTransform::SetLocalMatrix(math::float4x4 newLocal)
{
	localMatrix = newLocal;
}

math::float4x4 ComponentTransform::GetGlobalMatrix()
{
	return globalMatrix;
}

void ComponentTransform::SetGlobalMatrix(float4x4 newGlobal)
{
	globalMatrix = newGlobal;
}

void ComponentTransform::CalculateGlobalMatrix(float4x4 newLocal)
{
	if (parent != nullptr)
	{
		ComponentTransform* parentTranform = (ComponentTransform*)parent->GetComponent(TRANSFORM);
		if (parentTranform != nullptr)
		{
			globalMatrix = parentTranform->globalMatrix * newLocal;
		}
	}
	else
	{
		globalMatrix = newLocal;
	}
}

void ComponentTransform::SetTransformation(math::float4x4 localM)
{
	if (parent->parent != nullptr)
	{
		ComponentTransform* parentTranform = (ComponentTransform*)parent->parent->GetComponent(TRANSFORM);
		if (parentTranform != nullptr)
		{
			globalMatrix = parentTranform->globalMatrix * localM;
		}
	}
	else
	{
		globalMatrix = localM;
	}

	localMatrix = localM;
}

void ComponentTransform::SaveComponent(JSON_Object* object)
{

}

