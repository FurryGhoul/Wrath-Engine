#include "Component.h"
#include "GameObject.h"

Component::Component() {}

Component::Component(Component_Type type, GameObject* parent)
{
	c_type = type;
	this->parent = parent;
}

Component::~Component() {}

Component_Type Component::GetCompType() { return c_type; }

void Component::ComponentUpdate() {}
