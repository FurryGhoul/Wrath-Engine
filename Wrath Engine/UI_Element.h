#pragma once
class Application;

#include "Globals.h"
#include "ImGui/imgui.h"
#include <list>

using namespace std;

class UI_Element
{
public:
	Application* App = nullptr;

	UI_Element(Application* app, bool active = true) :App(app), show(active) {}
	~UI_Element() {}


	virtual bool Start() { return true; }

	virtual update_status PreUpdate(float dt) { return UPDATE_CONTINUE; }

	virtual update_status Update(float dt) { return UPDATE_CONTINUE; }

	virtual update_status PostUpdate(float dt) { return UPDATE_CONTINUE; }

	virtual void Draw(bool* open) {}

	virtual bool CleanUp() { return true; }

	//string elem_name = NULL;
	bool show = true;
};