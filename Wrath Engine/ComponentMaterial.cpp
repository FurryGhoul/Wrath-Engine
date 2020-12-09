#include "Application.h"
#include "ModuleFileSystem.h"
#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(){}

ComponentMaterial::ComponentMaterial(Component_Type type, GameObject* parent) : Component(type, parent) {}

ComponentMaterial::~ComponentMaterial() {}

void ComponentMaterial::SaveComponent(JSON_Object* object)
{
	App->file_system->AddInt(object, "Type:", 2);
	App->file_system->AddString(object, "Path:", path.c_str());
}
