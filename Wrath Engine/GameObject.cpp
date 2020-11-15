#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

GameObject::GameObject() 
{ 
	AddComponent(TRANSFORM); 
	this->parent = nullptr;
	this->name = "";
}

GameObject::GameObject(GameObject* parent, string name)
{
	AddComponent(TRANSFORM);
	this->parent = parent;
	this->name = name;
}

GameObject::~GameObject() {}

void GameObject::Update()
{
	for (list<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter) { (*iter)->ComponentUpdate(); }
}

GameObject* GameObject::AddChildren(std::string name)
{
	GameObject* ret = new GameObject(this, name);
	children.push_back(ret);
	return ret;
}

void GameObject::AddParent(GameObject* newparent, GameObject* child)
{
	if (newparent != parent) child->parent = newparent;
}

Component* GameObject::AddComponent(Component_Type comp_type)
{
	Component* ret;

	switch (comp_type)
	{
	case NO_TYPE:
		break;

	case MESH:
		ret = new ComponentMesh(comp_type, this);
		components.push_back(ret);
		break;

	case MATERIAL:
		ret = new ComponentMaterial(comp_type, this);
		components.push_back(ret);
		break;

	case TRANSFORM:
		ret = new ComponentTransform(comp_type, this);
		components.push_back(ret);
		break;

	case CAMERA:
		ret = new ComponentCamera(comp_type, this);
		components.push_back(ret);
		break;
	}

	return ret;
}

Component* GameObject::GetComponent(Component_Type comp_type)
{
	Component* ret = nullptr;

	for (auto item = components.begin(); item != components.end(); item++)
	{
		if ((*item)->GetCompType() == comp_type)
		{
			ret = (*item);
		}
	}
	return ret;
}

