#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInterface.h"

#include "UI_Config.h"
#include "UI_Scene.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleInterface* ui;

	UI_Config* configuration = nullptr;
	UI_Scene* scene_ui = nullptr;

private:

	Timer	ms_timer;
	float	dt;
	p2List<Module*> list_modules;
	list<UI_Element*> list_UImodules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(char* url);

	float milliseconds;

private:

	void AddModule(Module* mod);
	void AddUIModule(UI_Element* mod);
	void PrepareUpdate();
	void FinishUpdate();
};