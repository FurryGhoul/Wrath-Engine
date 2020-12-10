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

	App->file_system->AddVec3(object, "Translation", ((ComponentTransform*)GetComponent(TRANSFORM))->compTranslation);
	App->file_system->AddVec3(object, "Scale", ((ComponentTransform*)GetComponent(TRANSFORM))->compScale);
	App->file_system->AddVec4(object, "Rotation", ((ComponentTransform*)GetComponent(TRANSFORM))->compRotation);

	int numComponents = -1;
	for (list<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		numComponents++;
	}

	App->file_system->AddInt(object, "Number of Components", numComponents);

	JSON_Value* objectComponents = json_value_init_array();
	json_object_set_value(object, "Components", objectComponents);

	for (list<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		JSON_Value* objectComp = json_value_init_object();
		(*iter)->SaveComponent(json_object(objectComp));
		json_array_append_value(json_array(objectComponents), objectComp);
	}


}

GameObject* GameObject::LoadGameObject(JSON_Object* object)
{
	GameObject* objectToLoad = new GameObject();
	objectToLoad->uuid = App->file_system->GetInt(object, "UUID");
	objectToLoad->parentUUID = App->file_system->GetInt(object, "Parent_UUID");
	objectToLoad->name = App->file_system->GetString(object, "Name");

	return objectToLoad;
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

