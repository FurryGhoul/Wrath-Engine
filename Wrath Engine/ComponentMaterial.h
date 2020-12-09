#pragma once

#include "Component.h"
#include <string>

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	ComponentMaterial(Component_Type type, GameObject* parent);
	~ComponentMaterial();

	void SaveComponent(JSON_Object* object);

public:
	uint textureID;
	uint width;
	uint height;
	std::string name;
	std::string path;
	uint depth;
	uint BPP;
};
