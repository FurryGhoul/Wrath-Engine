#pragma once
#include "Component.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Math/Quat.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	ComponentTransform(Component_Type type, GameObject* parent);
	~ComponentTransform();

	math::float4x4 GetLocalMatrix();
	void SetLocalMatrix(float4x4 newLocal);

	math::float4x4 GetGlobalMatrix();
	void SetGlobalMatrix(float4x4 newGlobal);

	void SetTransformation(math::float4x4 localM);
	
private:
	math::float4x4 localMatrix;
	math::float4x4 globalMatrix;
};

