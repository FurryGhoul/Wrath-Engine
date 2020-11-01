#pragma once
#include "Component.h"
#include "MathGeoLib/Math/float4x4.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	ComponentTransform(Component_Type type, GameObject* parent);
	~ComponentTransform();

	math::float4x4 GetLocalMatrix();

private:
	math::float4x4 local;
};

