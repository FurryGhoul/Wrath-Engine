#include "Application.h"
#include "ModuleFileSystem.h"
#include "ComponentMesh.h"

#include "Glew/include/glew.h"

ComponentMesh::ComponentMesh() {}

ComponentMesh::ComponentMesh(Component_Type type, GameObject* parent) : Component(type, parent) {}

ComponentMesh::~ComponentMesh() 
{
	delete[] indices;
	delete[] vertices;
	delete[] normals;
	delete[] texture_coords;

	indices = nullptr;
	vertices = nullptr;
	normals = nullptr;
	texture_coords = nullptr;

	glDeleteBuffers(1, &id_indices);
	glDeleteBuffers(1, &id_texcoords);
}

void ComponentMesh::SaveComponent(JSON_Object* object)
{
	App->file_system->AddInt(object, "Type:", 1);
	//App->file_system->AddString(object, "Path:", path.c_str());
}

ComponentMesh* ComponentMesh::LoadComponent(JSON_Object* object)
{
	ComponentMesh* mesh = new ComponentMesh();
	App->file_system->GetString(object, "Path:");
	return mesh;
}


