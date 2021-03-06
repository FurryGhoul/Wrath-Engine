#pragma once
#include "Globals.h"
#include "Parson/parson.h"

class GameObject;

class Component
{
public:
	Component();
	Component(Component_Type type, GameObject* parent);
	virtual~Component();

	Component_Type GetCompType();

	virtual bool ComponentUpdate();

	virtual void SaveComponent(JSON_Object* object);

public:
	Component_Type c_type;

	GameObject* parent;
};
