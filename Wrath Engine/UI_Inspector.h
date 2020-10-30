#pragma once
#include "UI_Element.h"

class GameObject;
class ComponentMesh;

class UI_Inspector : public UI_Element
{
public:
	UI_Inspector(Application* app, bool start_enabled = true);
	~UI_Inspector();


	void Draw(bool* open);

public:
	GameObject* selectedGO = nullptr;
	ComponentMesh* meshes = nullptr;

	int numChildren = 0;

	int meshPos[3] = { 0, 0, 0 };
};
