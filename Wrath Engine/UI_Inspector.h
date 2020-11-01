#pragma once
#include "UI_Element.h"

class GameObject;
class ComponentMesh;
class ComponentMaterial;

class UI_Inspector : public UI_Element
{
public:
	UI_Inspector(Application* app, bool start_enabled = true);
	~UI_Inspector();


	void Draw(GameObject* selectedGO, bool* open);

public:

	bool normals = false;
};
