#pragma once
#include "Component.h"
#include "GameObject.h"

class ComponentMaterial;
class ComponentMesh : public Component
{
public:
	ComponentMesh();
	ComponentMesh(Component_Type type, GameObject* parent);

	~ComponentMesh();

public:
	// index in VRAM
	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	//Normals
	uint* normals = nullptr;
	uint num_normals = 0;
	uint id_normals = 0;

	// unique vertex in VRAM
	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	// Textures
	uint id_texcoords = 0;
	uint texture = 0;
	uint texture_width = 0;
	uint texture_height = 0;
	float* texture_coords = nullptr;
};
