#include "Application.h"
#include "ModuleFileSystem.h"

#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "Parson/parson.h"

GameObject::GameObject() 
{ 
	uuid = Generate_UUID();
	this->parent = nullptr;
	this->name = "";
}

GameObject::GameObject(GameObject* parent, string name)
{
	uuid = Generate_UUID();
	if (parent)
	{
		parentUUID = parent->uuid;
	}
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

void GameObject::SaveGameObject(JSON_Object* object)
{
	App->file_system->AddInt(object, "UUID", uuid);
	if (parent)
	{
		App->file_system->AddInt(object, "Parent_UUID", parentUUID);
	}
	App->file_system->AddString(object, "Name", name.c_str());

	math::float4x4 transform = ((ComponentTransform*)GetComponent(TRANSFORM))->GetGlobalMatrix();
	App->file_system->AddVec3(object, "Translation", transform.TranslatePart());
	App->file_system->AddVec3(object, "Scale", transform.ExtractScale());
	App->file_system->AddVec3(object, "Rotation", transform.RotatePart().ToEulerXYZ());

	JSON_Value* objectComponents = json_value_init_array();
	json_object_set_value(object, "Components", objectComponents);

	for (list<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		JSON_Value* objectComp = json_value_init_object();
		(*iter)->SaveComponent(json_object(objectComp));
		json_array_append_value(json_array(objectComponents), objectComp);
	}
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

