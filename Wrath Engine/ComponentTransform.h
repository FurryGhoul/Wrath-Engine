#pragma once
#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* parent);
	~ComponentTransform();
};

