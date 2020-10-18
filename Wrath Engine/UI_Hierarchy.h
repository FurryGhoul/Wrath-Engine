#pragma once
#include "UI_Element.h"
#include "SDL/include/SDL.h"

class GameObject;

class UI_Hierarchy : public UI_Element
{
public:
	UI_Hierarchy(Application* app, bool start_enabled = true);
	~UI_Hierarchy();

	update_status Update(float dt);

	void Draw(bool* open);

public:
	bool createCube = false;
	bool createSphere = false;
	bool createPlane = false;

	GameObject* selectedGO = nullptr;

private:

	int numberPrimitives = 0;
	const char* prim_name;

	int prim_posx, prim_posy, prim_posz;
	int prim_sizeX, prim_sizeY;

};

